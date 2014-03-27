<?php

/*
Plugin Name: Twitter Digest
Plugin URI: http://whalespine.org/projects/wordpress/twitter-digest
Description:  Create a daily or weekly post on your blog containing the tweets from your <a href="http://twitter.com">Twitter</a> account.

Version: 2.9 mod for twitter status link
Author: Tim Beck, mod by Akihiko SHIRAI
Author URI: http://whalespine.org
*/

// Copyright (c) 2009 - 2010 Tim Beck and Paul Wlodarczyk. All rights reserved.
//
// Released under the GPL license
// http://www.opensource.org/licenses/gpl-license.php
//
// This is an add-on for WordPress
// http://wordpress.org/
//
// **********************************************************************
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// **********************************************************************
//
//=======================
// Defines

define('WS_FETCH_SPACE', 30); //X seconds between fetches 
define('WS_TWITTER_LIMIT', 200); //the most tweets you can pull from the Twitter REST API  
define('WS_API_USER_TIMELINE', 'https://api.twitter.com/1.1/statuses/user_timeline.json');
define('WS_STATUS_URL', 'https://twitter.com/###USERNAME###/statuses/###STATUS###');
define('WS_PROFILE_URL', 'https://twitter.com/###USERNAME###');
define('WS_HASHTAG_URL', 'https://twitter.com/search?q=###HASHTAG###&src=hash');
define('WS_VERSION', '2.9');

    
//========================
// Digest cron activation

register_activation_hook(__FILE__, 'ws_activate_digest');
register_deactivation_hook(__FILE__, 'ws_deactivate_digest');
add_action('ws_td_digest_event', 'ws_ping_twitter');

function ws_activate_digest() {
  wp_schedule_event(time(), 'hourly', 'ws_td_digest_event');
}

function ws_deactivate_digest() {
  wp_clear_scheduled_hook('ws_td_digest_event');
}

// Check Twitter to see if there are any new tweets that need to be published.
// Return values:
// null -> all good
// 1 -> Error w/ twitter
// 2 -> Too soon
// 3 -> Missing username/password
// 4 -> Post created and published now or put in draft (depending on status option)
// 5 -> No tweets to post
// 6 -> Post created but not published until later
// 7 -> Wrong day of week

function ws_ping_twitter() {

  // Do we have a username?
  if (!get_option("ws_td_username")) {
    return 3;
  }

  // We don't need the password anymore, so delete it.
  if (get_option("ws_td_password")) {
     delete_option("ws_td_password");
  }
   
  // Has there been enough time since the last check? This avoids a race
  // condition which would produce a duplicate post.

  $last = get_option('ws_td_last_check');
  if (time() - $last < WS_FETCH_SPACE) {
    return 2;
  }
  update_option("ws_td_last_check", time());

  // Is this a daily or weekly post?

  // Need to know today's local midnight to work out start/end times
  $midnight = strtotime(date('Y-m-d, 00:00:00', time() + ws_gmtOffset()));
  
  $pub_day = get_option('ws_td_pub_day');
  $pub_freq = get_option('ws_td_freq');
  $current_day_of_week = date("w", $midnight);

  // Test if it's weekly, then if so is today the right day of the week. If
  // not, return; pub_freq=0 is daily
  if ($pub_freq == 1) {
    if ($current_day_of_week != $pub_day) {
      return 7;
    } else {
      // 7 days ago at midnight
      $startDate = strtotime('-7 days', $midnight);
    }
  } else {
    // yesterday at midnight
    $startDate = strtotime('-1 days', $midnight);
  }

  // yesterday at 11:59:59
  $endDate = strtotime('-1 second', $midnight);


  $startDateStr = date(ws_getDateFormat(), $startDate);
  $endDateStr = date(ws_getDateFormat(), $endDate);
  
  // Get the last tweet id from the last post
  $lastTweet = get_option('ws_td_last_tweet');
  if (!$lastTweet) {
    $lastTweet = 0;
    update_option('ws_td_last_tweet', $lastTweet);
  }

  // Get the tweets
  $numtweets = get_option('ws_td_num_tweets');
  // range check. Twitter limit is to 200
  if ($numtweets > WS_TWITTER_LIMIT) { 
    $numtweets = WS_TWITTER_LIMIT;
  }
  
  if ($numtweets == 0) {
    $numtweets = 20;  //default is 20 with no count argument, so fetch 20 if NULL
  }

  // get the last N tweets, since the last tweet
  $url = WS_API_USER_TIMELINE . "?screen_name=".get_option('ws_td_username') ."&count=". $numtweets ;

  if (get_option('ws_td_includeRTs')) {
    $url .= '&include_rts=1';
  }
  
  if ($lastTweet) {
    $url .= "&since_id=" . $lastTweet; 
  }

  
  // Fetch the tweets
  $tweets = ws_fetch_tweets($url);

  // Go through the array and process any tweets from the desired time period 
  $tweet_content = array();
  $newLastTweet = false;
  if ($tweets && count($tweets) >= 0) { 
    // process the tweets

    foreach ($tweets as $tw_data) {
      // Convert the time to local
      $tw_data->created_at = strtotime($tw_data->created_at) + ws_gmtOffset();


      // Was this tweet added in the time period of interest? 
      if ($tw_data->created_at < $startDate) {
        continue;
      }
      if ($tw_data->created_at > $endDate ) {
        continue;
      }

      // Are we dropping replies?
      if (get_option('ws_td_drop_replies') && preg_match('/^@.*/', $tw_data->text)) {
        continue;
      }
      
      // All good, so format and add to the content
      array_push($tweet_content, ws_format_tweet($tw_data)."\n\n");

      // Remember the first tweet in the list, which is actually the most
      // recently tweeted.
      if (!$newLastTweet) {
        $newLastTweet = $tw_data->id_str;
      }
    }

    
    // To avoid race conditions, check to make sure lastTweet hasn't changed
    if ($lastTweet == get_option('ws_td_last_tweet')) {

      $count = count($tweet_content);

      if ($count != 0 && $count >= get_option('ws_td_min_tweets')) {

        // Before we push the content, we need to put it in the right order
        if (get_option('ws_td_chrono') == 1) {
          $tweet_content = array_reverse($tweet_content);
        }   
        
        $title = get_option('ws_td_title');
        if (!$title) {
          $title = "Tweets for %startdate";
        }

        // We look for %date to be backwards compatible
        $title = preg_replace("/%date/", $startDateStr, $title);        
        $title = preg_replace("/%startdate/", $startDateStr, $title);
        $title = preg_replace("/%enddate/", $endDateStr, $title);        

        $excerpt = get_option('ws_td_excerpt');
        $excerpt = preg_replace("/%date/", $startDateStr, $excerpt);
        $excerpt = preg_replace("/%startdate/", $startDateStr, $excerpt);
        $excerpt = preg_replace("/%enddate/", $endDateStr, $excerpt);        
        
        $postcontent = "<ul class=\"ws_tweet_list\">\n\n";
        $postcontent .= join("\n\n", $tweet_content);
        $postcontent .= "\n</ul>";

        // This is messy, but good enough for now.
        if (ws_create_post($postcontent, $title, get_option('ws_td_pub_time'), $excerpt)
            == 1 /* Published in future */) {
          $retval = 6;
        } else {
          // Published now or drafted.
          $retval = 4;
        }
        
        // Update the last tweet id
        update_option('ws_td_last_tweet', $newLastTweet);

        return $retval;
        
      }
      else {
        return 5;
      }
    }
    
  }
  else {
    return 5;
  } 
   
}

// This function creates the actual post and schedules it for publishing time
// at $pubtime, if the status option is set to 'publish'. Otherwise the post
// is put in 'draft' status.
// Return values:
// 0: published now
// 1: published in future
// 2: drafted
function ws_create_post($content, $title, $pubtime, $excerpt) {

  global $wpdb;
  $result = 0;
  
  // Are we putting this in draft or publishing (now or later)?
  if (get_option('ws_td_status') == 'draft') {
    $status = 'draft';
    $result = 2;
  } else {

    $status='publish';
    
    // Are we doing this now or later?
    if ($pubtime) {
      $time = date('Y-m-d ').$pubtime.":00";
      $timestamp = strtotime($time);
      if ($timestamp > current_time('timestamp')) {
        $result = 1;
      } 
    } else {
      $time = current_time('mysql');
    }   
  }

  // Create the post
  $post_data = array(
                     'post_content' => $wpdb->escape($content),
                     'post_title' => $wpdb->escape($title),
                     'post_date' => $time,
                     'post_category' => array(get_option('ws_td_category')),
                     'post_status' => $status,
                     'post_author' => $wpdb->escape(get_option('ws_td_author')),
                     'post_excerpt' => $wpdb->escape($excerpt)
                     );
  
  // Insert post
  $post_id = wp_insert_post($post_data);
  add_post_meta($post_id, 'ws_tweeted', '1', true);
  
  // Tag it
  wp_set_post_tags($post_id, get_option('ws_td_post_tags'));

  return $result;
}

// Returns an html formatted $tweet.  This is almost directly borrowed from Twitter Tools
function ws_format_tweet($tweet) {
  
  $output = '<li class="ws_tweet">';
  $output .= ws_make_clickable(wp_specialchars($tweet->text));
  if (!empty($tweet->in_reply_to_screen_name)
      && (!empty($tweet->in_reply_to_status_id))) {
    $output .= 	' <a href="'.ws_status_url($tweet->in_reply_to_screen_name, $tweet->in_reply_to_status_id).'">'.sprintf(__('in reply to %s', 'twitter-digest'), $tweet->in_reply_to_screen_name).'</a>';
  }

  // Show the date/time if the options are selected
  $showTime = get_option('ws_td_showtime');
  if (!$showTime) { $showTime = 0; }
  // Show the date if the option is selectd
  $showDate = get_option('ws_td_showdate');
  if (!$showDate) { $showDate = 0; }
  
  $time_display = '';
  if ($showTime == 1) {
    $time_display = gmdate('H:i:s', $tweet->created_at);

    // Add the comma if we are going to show the date as well
    if ($showDate == 1) {
      $time_display .= ", ";
    }
  }

  // Add the date
  if ($showDate == 1) {
    $time_display .= gmdate('Y-m-d', $tweet->created_at);
  }

  // Use a small arrow for the status links if time and date are off
  if ($showDate != 1 && $showTime != 1) {
    $time_display = "->";
  }

  // Add the status link 
  $username = get_option('ws_td_username'); 
  $output .= ' <a class="ws_tweet_time" href="'.ws_status_url($username, $tweet->id_str).'">'.$time_display.'</a>';

      
  $output .= '</li>';
  return $output;
  
}

// Most of the following formatting functions are borrowed from Twitter Tool
function ws_make_clickable($tweet) {
  
  //	$tweet = preg_replace('/\@([a-zA-Z0-9_]{1,15}) /','@<a
  //	href="http://twitter.com/\\1">\\1</a> ', $tweet);
  $tweet = preg_replace_callback(
                                 '/((?:^|\s+|[\"\'\[\(]))\@([\w]{1,30})/',
                                 create_function(
                                                 '$matches',
                                                 'return $matches[1].ws_profile_link($matches[2]);'
                                                 ),
                                 $tweet
                                 );
  $tweet = preg_replace_callback(
                                 '/((?:^|\s+|[\"\'\[\(]))\#([\w]{1,30})/',
                                 create_function(
                                                 '$matches',
                                                 'return $matches[1].ws_hashtag_link($matches[2]);'
                                                 ),
                                 $tweet
                                 );

  // make_clickable doesn't handle (url) very well, so we'll chuck some spaces in
  $tweet = preg_replace("/\((http.*)\)/", "( $1 )", $tweet);

  return make_clickable($tweet);
}

function ws_status_url($username, $status) {
  return str_replace(
                     array('###USERNAME###', '###STATUS###'),
                     array($username, $status),
                     WS_STATUS_URL);
}

function ws_profile_link($username, $prefix = '@') {
  return '<a href="'.ws_profile_url($username).'">'.$prefix.$username.'</a>';
}

function ws_hashtag_link($hashtag, $prefix = '#') {
  return '<a href="'.ws_hashtag_url($hashtag).'">'.$prefix.htmlspecialchars($hashtag).'</a>';
}

function ws_profile_url($username) {
  return str_replace('###USERNAME###', $username, WS_PROFILE_URL);
}

function ws_hashtag_url($hashtag) {
  $hashtag = urlencode('#'.$hashtag);
  return str_replace('###HASHTAG###', $hashtag, WS_HASHTAG_URL);
}

// Based on Alex King's implementation for the Twitter Tools plugin
function ws_fetch_tweets($url) {

  // First get our oauth connection
  require_once("twitteroauth.php"); 
  $consumerkey =  get_option('ws_td_conskey'); // "3JucGrvAxOuaAYbLml5nkQ";
  $consumersecret = get_option('ws_td_conssecret'); //"iIRXb7c0VFwTYmI4TCNNIrUWB2YDxmL6c11YogcqQw0";
  $accesstoken = get_option('ws_td_acctoken'); // "14198382-SnzuP0AMz3qVWTolneB8QG7D1kHt6WNMosZzb48LE";
  $accesstokensecret = get_option('ws_td_accsecret'); // "9O7sc2tbea8PEx3Py9S4VWsHkbZtDBcP0n4HvnzokqM";

  
  if (!$consumerkey || !$consumersecret || !$accesstoken || !$accesstokensecret) {
    update_option('ws_td_error', 'Check your account information and make sure all token fields have a valid value.');
    return false;
  }
    
  $connection = new TwitterOAuth($consumerkey, $consumersecret,
                                 $accesstoken, $accesstokensecret);

  // Now make the request
  $tweets = $connection->get($url);

  // Check the error code
  if(empty($connection->http_code) || $connection->http_code != 200) {
    update_option('ws_td_error', 'Error retrieving tweets. <br/> Status: '.$connection->http_code.' Check your authentication tokens. <br/> '.print_r($connection->http_info, true));
     return false;
  }

  // Everything is ok
  update_option('ws_td_error','');
  
  return $tweets;
}

// Simply resets the 'ws_td_last_tweet' option
function ws_td_resetDatabase() {
  update_option('ws_td_last_tweet',0);   
}

// The menu hook
function ws_menu_item() {
  if (current_user_can('manage_options')) {
    add_options_page(
                     __('Twitter Digest Options', 'twitter-digest')
                     , __('Twitter Digest', 'twitter-digest')
                     , 'manage_options'
                     , basename(__FILE__)
                     , 'ws_options_form'
                     );
  }
}
add_action('admin_menu', 'ws_menu_item');


function ws_options_form() {

  // Check here if we are going to do the check now
  global $wpdb;

  // Reset the database if necessary
  if (isset($_POST["action"]) && $_POST["action"] == 'resetdb') {
     ws_td_resetDatabase();
  }

  // Get some variables together
  $ping_message = "(This may take a while if there are tweets to process.)";
  $ping_style="color: black";
  
  // Range check the publish day 
   $pub_day_value = get_option('ws_td_pub_day');
  if ($pub_day_value < 0 || $pub_day_value > 6) { $pub_day_value =  date("w"); }

  // yesterday or last week?
  if (get_option('ws_td_freq') == 0) {
    $periodStr = 'yesterday';
  } else {
    $periodStr = 'last week';
  }
  
  // Ping if necessary, and show the correct message.
  if (isset($_POST["action"]) && $_POST["action"] == "ping") {
    switch(ws_ping_twitter(true)) {
    case 1:
      $pwuser = get_option('ws_td_username');
      
      $ping_message = "Something went wrong with Twitter.  Check for an error message above. Current Twitter username is " . $pwuser. "." ;
      $ping_style = "color: red";
      break;
    case 2:
      $ping_message = "To keep things sane, we're going to wait ".WS_FETCH_SPACE." seconds between pings.";
      $ping_style = "color: red";
      break;
    case 3:
      $ping_message = "Missing Twitter username and/or password";
      $ping_style = "color: red";
      break;
    case 4:
      $ping_message = "Post containing ".$periodStr."'s tweets has been ".(get_option('ws_td_status'). 'ed.');
      $ping_style ="color: green";
      break;
    case 6: $ping_message = "Post containing ".$periodStr ."'s tweets scheduled.";
      $ping_style = "color: green";
      break;
    case 5: $ping_message = "No new tweets found from " . $periodStr;
	  break;
    case 7:
      $daylabel = date("l");
      $pub_day_str = ws_getDoW($pub_day_value);
      $now = date("H:i:s");
      $pub_time_str = get_option('ws_td_pub_time');
      if (!$pub_time_str) { $pub_time_str = '00:00:00'; }
      
      $ping_message = "Today is " . $daylabel . " and the local time is " . $now . " and you are configured to publish on " . $pub_day_str . " at " . $pub_time_str . ".";
     $ping_style = "color: red";
    }
  }
  
  // Get all the options together
  $categories = get_categories('hide_empty=0');
  $cat_options = '';
  foreach ($categories as $category) {
    if ($category->term_id == get_option('ws_td_category')) {
      $selected = 'selected="selected"';
    }
    else {
      $selected = '';
    }
    $cat_options .= "\n\t<option value='$category->term_id' $selected>$category->name</option>";
  }

  // get_users_of_blog is deprecated.
  $authors = get_users_of_blog();
  $author_options = '';
  foreach ($authors as $user) {
    $usero = new WP_User($user->user_id);
    $author = $usero->data;
    // Only list users who are allowed to publish
    if (! $usero->has_cap('publish_posts')) {
      continue;
    }
    if ($author->ID == get_option('ws_td_author')) {
      $selected = 'selected="selected"';
    }
    else {
      $selected = '';
    }
    $author_options .= "\n\t<option value='$author->ID' $selected>$author->user_nicename</option>";
  }
  
  // Set up the options for the status. Just draft or publish for now.
  $status_options = '';
  if (get_option('ws_td_status') == 'draft') {
    $status_options ='
       <option value="publish">'.__('Publish', 'twitter-digest').'</option>
       <option value="draft" selected="selected">'.__('Draft', 'twitter-digest').'</option>
     '; 
  } else {
    $status_options ='
       <option value="publish" selected="selected">'.__('Publish', 'twitter-digest').'</option>
       <option value="draft">'.__('Draft', 'twitter-digest').'</option>
     '; 
  }
  
  // Drop replies options
  $drop_replies_check = get_option('ws_td_drop_replies') == 1 ? 'checked="true"' : '';
  // Chrono options
  $chrono_check = get_option('ws_td_chrono') == 1 ? 'checked="true"' : '';
  // Show date options
  $showDate_check = get_option('ws_td_showdate') == 1 ? 'checked="true"' : '';
  $showTime_check = get_option('ws_td_showtime') == 1 ? 'checked="true"' : ''; 
  $includeRTs_check = get_option('ws_td_includeRTs') == 1 ? 'checked="true"' : ''; 
  
  // Publishing frequency options
  if (get_option('ws_td_freq') == 1) {
    $freq_options = '
       <option value="0">'.__('Daily', 'twitter-digest').'</option>
       <option value="1" selected="selected">'.__('Weekly', 'twitter-digest').'</option>
     ';

    $pub_day_display = '';
  }  else {
    $freq_options = '
      <option value="0" selected="selected">'.__('Daily', 'twitter-digest').'</option>
      <option value="1">'.__('Weekly', 'twitter-digest').'</option>
    ';
    $pub_day_display = 'display: none';
  }

  // DoW option
  $pub_day_options = '';
  for ($i = 0; $i < 7; $i++) {
    $pub_day_options .= '<option value="' . $i . '"';
    if ($i == $pub_day_value) {
      $pub_day_options .= ' selected="selected"';
    }
    $pub_day_options .= '>' . ws_getDow($i) . '</option>';
  }
  
  // Default the min tweet value to 1
  $min_tweets_value = get_option('ws_td_min_tweets');
  if ($min_tweets_value < 1) { $min_tweets_value = 1; }

  // Default the number of tweets to 20
  $num_tweets_value = get_option('ws_td_num_tweets');
  if (!$num_tweets_value) { $num_tweets_value =  20; }

  print('
  
  <style>
  
  p.submit {
  padding: 5px;
margin-top: 20px;
  }
  div.ws_td_error {
  margin: 20px 100px 20px 50px;    
  padding: 10px;
  border: 2px solid red;
    background-color: #FFEEEE;
  }
  .option {margin: 10px;}
  .section {
    border-bottom: 1px dashed #888;
    margin-bottom: 10px;
    padding-bottom: 10px;
  }
  fieldset h3 {
    color: #888;
  }
  </style>


  <script text="text/javascript">
    var ws = {
      // Toggle the day of week option
      toggleDoWOption: function(select) {

         if (jQuery(select).val() == 0) {
            jQuery("#dowOptionDiv").hide();
        } else {
            jQuery("#dowOptionDiv").show();
        }
      }
    }
  </script>

  <div class="wrap">
  <h2>'.__('Twitter Digest Options', 'twitter-tools').'</h2>
  
  '.ws_getErrorMessage().'
    <hr/><form id="ws_twittertools" name="ws_twittertools" action="options.php" method="post">
  '.wp_nonce_field('update-options').'
        
    <input type="hidden" name="action" value="update" />
    <input type="hidden" name="page_options" value="ws_td_username,ws_td_category,ws_td_author,ws_td_title,ws_td_drop_replies,ws_td_post_tags,ws_td_min_tweets,ws_td_chrono,ws_td_pub_time,ws_td_pub_day,ws_td_freq,ws_td_num_tweets,ws_td_showdate,ws_td_showtime, ws_td_excerpt, ws_td_dateFormat, ws_td_checktest, ws_td_status, ws_td_includeRTs, ws_td_conskey, ws_td_conssecret, ws_td_acctoken, ws_td_accsecret"/>
  
  <fieldset class="options section">
  <h3>Twitter Account Info</h3>
  <div class="option">
    <label for="ws_td_username">'.__('Username', 'twitter-digest').': </label>
    <input type="text" size="25" name="ws_td_username" id="ws_td_username" value="'.get_option('ws_td_username').'" autocomplete="off" />
  </div>

  <div class="option">
    <label for="ws_td_conskey">'.__('Consumer Key', 'twitter-digest').': </label>
    <input type="text" size="25" name="ws_td_conskey" id="ws_td_conskey" value="'.get_option('ws_td_conskey').'" autocomplete="off" />
  </div>

  <div class="option">
    <label for="ws_td_conssecret">'.__('Consumer Secret', 'twitter-digest').': </label>
    <input type="text" size="60" name="ws_td_conssecret" id="ws_td_conssecret" value="'.get_option('ws_td_conssecret').'" autocomplete="off" />
  </div>

  <div class="option">
    <label for="ws_td_acctoken">'.__('Access Token', 'twitter-digest').': </label>
    <input type="text" size="60" name="ws_td_acctoken" id="ws_td_acctoken" value="'.get_option('ws_td_acctoken').'" autocomplete="off" />
  </div>

  <div class="option">
    <label for="ws_td_accsecret">'.__('Access Token Secret', 'twitter-digest').': </label>
    <input type="text" size="60" name="ws_td_accsecret" id="ws_td_accsecret" value="'.get_option('ws_td_accsecret').'" autocomplete="off" />
  </div>

  </fieldset>

  <fieldset class="options section">
  <h3>Publish Options</h3>

  <div class="option">
    <label for="ws_td_freq">'.__('Frequency', 'twitter-digest').': </label>
    <select name="ws_td_freq" id="ws_td_freq" onchange="ws.toggleDoWOption(this);">'.$freq_options.'</select>
  </div>

  <div class="option">
  <label for="ws_td_pub_time">'.__('Time of Day:', 'twitter-digest').'</label>
  <input name="ws_td_pub_time" id="ws_td_pub_time" style="text-align:right" size="5" value="'.get_option('ws_td_pub_time').'">
  <span>'._('Specify the time at which the tweet post should be published in 24 hour format e.g. 2:22pm = 14:22 or leave empty for ASAP').'</span>
  </div>
  <div class="option" id="dowOptionDiv" style="'. $pub_day_display.'">
  <label for="ws_td_pub_day">'.__('Day of Week', 'twitter-digest').'</label>
  <select name="ws_td_pub_day" id="ws_td_pub_day">' . $pub_day_options . '</select>

  <span>'._('Specify the day of the week the post should be published in number format (0-6). e.g. Sunday = 0').'</span>
  </div>
  

   </fieldset>

  <fieldset class="options section">
  <h3>Post Options</h3>

  <div class="option">
  <label for="ws_td_title">'.__('Post Title:', 'twitter-digest').'</label>
  <input size="50" name="ws_td_title" id="ws_td_title" value="'.get_option('ws_td_title').'">
  <span>'._('Use %startdate and %enddate to insert the date').'</span>
  </div>

  <div class="option">
  <label for="ws_td_excerpt">'.__('Post Excerpt:', 'twitter-digest').'</label>
  <input size="75" name="ws_td_excerpt" id="ws_td_excerpt" value="'.get_option('ws_td_excerpt').'">
  <span>'._('Use %startdate and %enddate to insert the dates').'</span>
  </div>

  <div class="option">
  <label for="ws_td_dateFormat">'.__('Post Title/Excerpt Date Format:', 'twitter-digest').'</label>
  <input type="text" size="10" name="ws_td_dateFormat" id="ws_td_dateFormat" value="'.ws_getDateFormat().'"/>
  <span>'._('Defaults to Y-m-d').'</span>
  </div>

  <div class="option">
  <label for="ws_td_category">'.__('Post Category:', 'twitter-digest').'</label>
  <select name="ws_td_category" id="ws_td_category">'.$cat_options.'</select>
  </div>
  <div class="option">
  <label for="ws_td_post_tags">'.__('Post Tags:', 'twitter-digest').'</label>
  <input size="50" name="ws_td_post_tags" id="ws_td_post_tags" value="'.get_option('ws_td_post_tags').'">
  <span>'._('Separate multiple tags with commas. Example: tweets, twitter').'</span>
  </div>

  <div class="option">
  <label for="ws_td_author">'.__('Post Author:', 'twitter-digest').'</label>
  <select name="ws_td_author" id="ws_blog_post_author">'.$author_options.'</select>
  </div>

  <div class="option">
  <label for="ws_td_status">'.__('Post Status:', 'twitter-digest').'</label>
  <select name="ws_td_status" id="ws_blog_post_">'.$status_options.'</select>
  </div>

  <div class="option">
  <label for="ws_td_drop_replies">'.__('Exclude @reply tweets from the digest?', 'twitter-digest').'</label>
  <input value="1" type="checkbox" name="ws_td_drop_replies" id="ws_td_drop_replies" '.$drop_replies_check.'/_check>
  </div>
  <div class="option">
  <label for="ws_td_chrono">'.__('Show tweets in chronological order?', 'twitter-digest').'</label>
  <input value="1" type="checkbox" name="ws_td_chrono" id="ws_td_chrono" '.$chrono_check.'/>
  </div>
  <div class="option">
  <label for="ws_td_showdate">'.__('Show tweet dates?', 'twitter-digest').'</label>
  <input value="1" type="checkbox" name="ws_td_showdate" id="ws_td_showdate" '.$showDate_check.'/>
  </div> 

  <div class="option">
  <label for="ws_td_showtime">'.__('Show tweet times?', 'twitter-digest').'</label>
  <input value="1" type="checkbox" name="ws_td_showtime" id="ws_td_showtime" '.$showTime_check.'/>
  </div>

  <div class="option">
  <label for="ws_td_includeRTs">'.__('Include retweets?', 'twitter-digest').'</label>
  <input value="1" type="checkbox" name="ws_td_includeRTs" id="ws_td_includeRts" '.$includeRTs_check.'/>
  </div>  

  <div class="option">
  <label for="ws_td_min_tweets">'.__('Minimum number of tweets required for post: ', 'twitter-digest').'</label>
  <input name="ws_td_min_tweets" id="ws_td_min_tweets" size="1" style="text-align: right" value="'.$min_tweets_value.'">
  </div>
  
  <div class="option">
  <label for="ws_td_num_tweets">'.__('Maximum number of tweets to retrieve (Twitter caps at 200): ', 'twitter-digest').'</label>
  <input name="ws_td_num_tweets" id="ws_td_num_tweets" size="3" style="text-align: right" value="'.$num_tweets_value.'">
  </div>
  </fieldset>
  <div class="section">
  <p class="submit">
  <input type="submit" name="submit" value="'.__('Update Options', 'twitter-digest').'" />
  </p>
  </div>

  </form>

   <div class="section">
     <form method="POST">
       <p class="submit" style="margin: 0;">
        <input type="hidden" name="action" value="ping"/>
       <input type="submit" name="submit" value="'.__('Ping Twitter', 'twitter-digest').'" />
       <span style="'.$ping_style.'">'.$ping_message.'</span>
  
       </p>
      </form>
      <form method="POST">
      <p class="submit" style="margin: 0;">
       <input type="hidden" name="action" value="resetdb"/>   
       <input type="submit" name="submit" value="'.__('Reset Database', 'twitter-digest').'" />
        <span>Clicking this button resets the Twitter Digest database and may result in duplicate posts.</span>
      </p>
     </form>
   </div>
  
</div>
  ');
}


// Get the date option or fall back to the default
function ws_getDateFormat() {
  $format = get_option('ws_td_dateFormat');
  if ($format == '') {
    $format = 'Y-m-d';
  }
  return $format;
}

// Get the string rep of a day given the index. Sunday = 0;
function ws_getDoW($num) {
  switch($num) {
  case 0:
    return "Sunday";
    break;
  case 1:
    return "Monday";
    break;
  case 2:
    return "Tuesday";
    break;
  case 3:
    return "Wednesday";
    break;
  case 4:
    return "Thursday";
    break;
  case 5: 
    return "Friday";
    break;
  case 6: 
    return "Saturday";
  }
  
}

// Returns the gmt offset for Wordpress in seconds
function ws_gmtOffset() {
  return get_option('gmt_offset') * 3600;
}

function ws_gmtTime() {
  return time() - date("Z");
}

function ws_getErrorMessage() {
  $error = get_option('ws_td_error');
  if ($error) {
    return '<div class="ws_td_error">'.$error.'</div>';
  }
}

define('MYPLUGIN_FOLDER', dirname(__FILE__));

