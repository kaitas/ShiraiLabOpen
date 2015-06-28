// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// entity.cpp v2.0
// Last modification: Apr-14-2013

#include "entity.h"

//=============================================================================
// constructor
//=============================================================================
Entity::Entity() : Image()
{
    radius = 1.0;
    edge.left = -1;
    edge.top = -1;
    edge.right = 1;
    edge.bottom = 1;
    mass = 1.0;
    velocity.x = 0.0;
    velocity.y = 0.0;
    deltaV.x = 0.0;
    deltaV.y = 0.0;
    active = true;                  // the entity is active
    rotatedBoxReady = false;
    collisionType = entityNS::CIRCLE;
    health = 100;
    gravity = entityNS::GRAVITY;
    pixelsColliding = 0;
}

//=============================================================================
// Initialize the Entity.
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//      *textureM = pointer to TextureManager object
// Post: returns true if successful, false if failed
//=============================================================================
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    input = gamePtr->getInput();                // the input system
    audio = gamePtr->getAudio();                // the audio system
    return(Image::initialize(gamePtr->getGraphics(), width, height, ncols, textureM));
}

//=============================================================================
// activate the entity
//=============================================================================
void Entity::activate()
{
    active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Entity::update(float frameTime)
{
    velocity += deltaV;
    deltaV.x = 0;
    deltaV.y = 0;
    Image::update(frameTime);
    rotatedBoxReady = false;    // for rotatedBox collision detection
}

//=============================================================================
// ai (artificial intelligence)
// typically called once per frame
// performs ai calculations, ent is passed for interaction
//=============================================================================
void Entity::ai(float frameTime, Entity &ent)
{}

//=============================================================================
// Perform collision detection between this entity and the other Entity.
// Each entity must use a single collision type. Complex shapes that require
// multiple collision types may be done by treating each part as a separate
// entity or by using PIXEL_PERFECT collision.
// Typically called once per frame.
// The collision types: CIRCLE, BOX, ROTATED_BOX or PIXEL_PERFECT.
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collidesWith(Entity &ent, VECTOR2 &collisionVector)
{ 
    // if either entity is not active then no collision may occcur
    if (!active || !ent.getActive())    
        return false;

    // If both entities are using CIRCLE collision
    if (collisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE)
        return collideCircle(ent, collisionVector);
    // If both entities are using BOX collision
    if (collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
        return collideBox(ent, collisionVector);
    // If either entity is using PIXEL_PERFECT collision
    if (collisionType == entityNS::PIXEL_PERFECT || ent.getCollisionType() == entityNS::PIXEL_PERFECT)
        return collidePixelPerfect(ent, collisionVector);
    // All other combinations use separating axis test
    // If neither entity uses CIRCLE collision
    if (collisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
        return collideRotatedBox(ent, collisionVector);
    else    // one of the entities is a circle
        if (collisionType == entityNS::CIRCLE)  // if this entity uses CIRCLE collision
        {
            // Check for collision from other box with our circle
            bool collide = ent.collideRotatedBoxCircle(*this, collisionVector); 
            // Put the collision vector in the proper direction
            collisionVector *= -1;              // reverse collision vector
            return collide;
        }
        else    // the other entity uses CIRCLE collision
            return collideRotatedBoxCircle(ent, collisionVector);
    return false;
}

//=============================================================================
// Circular collision detection method
// Called by collision(), default collision detection method
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideCircle(Entity &ent, VECTOR2 &collisionVector)
{
    // difference between centers
    distSquared = *getCenter() - *ent.getCenter();
    distSquared.x = distSquared.x * distSquared.x;      // difference squared
    distSquared.y = distSquared.y * distSquared.y;

    // Calculate the sum of the radii (adjusted for scale)
    sumRadiiSquared = (radius*getScale()) + (ent.radius*ent.getScale());
    sumRadiiSquared *= sumRadiiSquared;                 // square it

    // if entities are colliding
    if(distSquared.x + distSquared.y <= sumRadiiSquared)
    {
        // set collision vector
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;   // not colliding
}

//=============================================================================
// Axis aligned bounding box collision detection method
// Called by collision()
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideBox(Entity &ent, VECTOR2 &collisionVector)
{
    // if either entity is not active then no collision may occcur
    if (!active || !ent.getActive())
        return false;

    // Check for collision using Axis Aligned Bounding Box.
    if( (getCenterX() + edge.right*getScale() >= ent.getCenterX() + ent.getEdge().left*ent.getScale()) && 
        (getCenterX() + edge.left*getScale() <= ent.getCenterX() + ent.getEdge().right*ent.getScale()) &&
        (getCenterY() + edge.bottom*getScale() >= ent.getCenterY() + ent.getEdge().top*ent.getScale()) && 
        (getCenterY() + edge.top*getScale() <= ent.getCenterY() + ent.getEdge().bottom*ent.getScale()) )
    {
        // set collision vector
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// Rotated Box collision detection method
// Called by collision()
// Uses Separating Axis Test to detect collision. 
// The separating axis test:
//   Two boxes are not colliding if their projections onto a line do not overlap.
// The current entity is A the other entity is B
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideRotatedBox(Entity &entB, VECTOR2 &collisionVector)
{
    float overlap01, overlap03;
    computeRotatedBox();                    // prepare rotated box
    entB.computeRotatedBox();               // prepare rotated box
    if (projectionsOverlap(entB, collisionVector) && entB.projectionsOverlap(*this, collisionVector))
    {
        // If we get to here the entities are colliding. The edge with the
        // smallest overlapping section is the edge where the collision is
        // occuring. The collision vector is created perpendicular to the
        // collision edge. The projection edges are 01 and 03.
        //
        //                    entA01min
        //                   /     entB01min
        //                  /     / entA01max 
        //                 /     / /  entB01max
        //                /     / /  /
        //            0--------------------1
        // entB03min..|          ____  
        // entA03min..|    _____|_ B | 
        //            |   | A   | |  |
        // entA03max..|   |_____|_|  |
        // entB03max..|         |____|
        //            | 
        //            |   
        //            3
        //            

        if (entA01min < entB01min)   // if A left of B
        {
            overlap01 = entA01max - entB01min;
            collisionVector = corners[1] - corners[0];
        }
        else    // else, A right of B
        {
            overlap01 = entB01max - entA01min;
            collisionVector = corners[0] - corners[1];
        }
        if (entA03min < entB03min)   // if A above B
        {
            overlap03 = entA03max - entB03min;
            if (overlap03 < overlap01)
                collisionVector = corners[3] - corners[0];
        }
        else    // else, A below B
        {
            overlap03 = entB03max - entA03min;
            if (overlap03 < overlap01)
                collisionVector = corners[0] - corners[3];
        }
        return true;
    }
    return false;
}

//=============================================================================
// Projects other box onto this edge01 and edge03.
// Called by collideRotatedBox()
// The current entity is A the other entity is B
// Post: returns true if projections overlap, false otherwise
//=============================================================================
bool Entity::projectionsOverlap(Entity &entB, VECTOR2 &collisionVector)
{
    float projection;

    // project other box onto edge01
    projection = graphics->Vector2Dot(&edge01, entB.getCorner(0)); // project corner 0
    entB01min = projection;
    entB01max = projection;
    // for each remaining corner
    for(int c=1; c<4; c++)
    {
        // project corner onto edge01
        projection = graphics->Vector2Dot(&edge01, entB.getCorner(c));
        if (projection < entB01min)
            entB01min = projection;
        else if (projection > entB01max)
            entB01max = projection;
    }
    // if projections do not overlap
    if (entB01min > entA01max || entB01max < entA01min)
        return false;                       // no collision is possible
    // project other box onto edge03
    projection = graphics->Vector2Dot(&edge03, entB.getCorner(0)); // project corner 0
    entB03min = projection;
    entB03max = projection;
    // for each remaining corner
    for(int c=1; c<4; c++)
    {
        // project corner onto edge03
        projection = graphics->Vector2Dot(&edge03, entB.getCorner(c));
        if (projection < entB03min)
            entB03min = projection;
        else if (projection > entB03max)
            entB03max = projection;
    }
    if (entB03min > entA03max || entB03max < entA03min) // if projections do not overlap
        return false;                       // no collision is possible
    return true;                            // projections overlap
}

//=============================================================================
// Rotated Box and Circle collision detection method
// Called by collision()
// Uses separating axis test on edges of box and radius of circle.
// If the circle center is outside the lines extended from the collision box
// edges (also known as the Voronoi region) then the nearest box corner is checked
// for collision using a distance check.
// The nearest corner is determined from the overlap tests.
//
//   Voronoi0 |   | Voronoi1
//         ---0---1---
//            |   |
//         ---3---2---
//   Voronoi3 |   | Voronoi2
//
// Pre: This entity (entA) must be rotated box and other entity (entB) must be circle.
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideRotatedBoxCircle(Entity &entB, VECTOR2 &collisionVector)
{
    float center01, center03, overlap01, overlap03;

    computeRotatedBox();                    // prepare rotated box

    // project circle center onto edge01
    center01 = graphics->Vector2Dot(&edge01, entB.getCenter());
    entB01min = center01 - entB.getRadius()*entB.getScale(); // min and max are Radius from center
    entB01max = center01 + entB.getRadius()*entB.getScale();
    if (entB01min > entA01max || entB01max < entA01min) // if projections do not overlap
        return false;                       // no collision is possible
        
    // project circle center onto edge03
    center03 = graphics->Vector2Dot(&edge03, entB.getCenter());
    entB03min = center03 - entB.getRadius()*entB.getScale(); // min and max are Radius from center
    entB03max = center03 + entB.getRadius()*entB.getScale();
    if (entB03min > entA03max || entB03max < entA03min) // if projections do not overlap
        return false;                       // no collision is possible

    // circle projection overlaps box projection
    // check to see if circle is in voronoi region of collision box
    if(center01 < entA01min && center03 < entA03min)    // if circle in Voronoi0
        return collideCornerCircle(corners[0], entB, collisionVector);
    if(center01 > entA01max && center03 < entA03min)    // if circle in Voronoi1
        return collideCornerCircle(corners[1], entB, collisionVector);
    if(center01 > entA01max && center03 > entA03max)    // if circle in Voronoi2
        return collideCornerCircle(corners[2], entB, collisionVector);
    if(center01 < entA01min && center03 > entA03max)    // if circle in Voronoi3
        return collideCornerCircle(corners[3], entB, collisionVector);

    // Circle not in voronoi region so it is colliding with edge of box.
    // The edge with the smallest overlapping section is the edge where the
    // collision is occuring. The collision vector is created perpendicular
    // to the collision edge. The projection edges are 01 and 03.
    //
    //                    entA01min
    //                   /   entB01min
    //                  /   /    entB01max 
    //                 /   /    /  entA01max
    //                /   /    /  /
    //            0--------------------1
    // entB03min..|        ___  
    // entA03min..|    ___/ B \__  
    // entB03max..|   |   \___/  |
    //            |   | A        |
    // entA03max..|   |__________|
    //            |         
    //            | 
    //            |   
    //            3
    //            
    if (entA01min < entB01min)   // if A left of B
    {
        overlap01 = entA01max - entB01min;
        collisionVector = corners[1] - corners[0];
    }
    else    // else, A right of B
    {
        overlap01 = entB01max - entA01min;
        collisionVector = corners[0] - corners[1];
    }
    if (entA03min < entB03min)   // if A above B
    {
        overlap03 = entA03max - entB03min;
        if (overlap03 < overlap01)
            collisionVector = corners[3] - corners[0];
    }
    else    // else, A below B
    {
        overlap03 = entB03max - entA03min;
        if (overlap03 < overlap01)
            collisionVector = corners[0] - corners[3];
    }
    return true;
}

//=============================================================================
// The box corner is checked for collision with circle using a distance check.
// Called by collideRotatedBoxCircle()
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector)
{
    distSquared = corner - *ent.getCenter();            // corner - circle
    distSquared.x = distSquared.x * distSquared.x;      // difference squared
    distSquared.y = distSquared.y * distSquared.y;

    // Calculate the sum of the radii, then square it
    sumRadiiSquared = ent.getRadius()*ent.getScale();   // (0 + circleR)
    sumRadiiSquared *= sumRadiiSquared;                 // square it

    // if corner and circle are colliding
    if(distSquared.x + distSquared.y <= sumRadiiSquared)
    {
        // set collision vector
        collisionVector = *ent.getCenter() - corner;
        return true;
    }
    return false;
}

//=============================================================================
// Compute corners of rotated box, projection edges and min and max projections
// 0---1  corner numbers
// |   |
// 3---2
//=============================================================================
void Entity::computeRotatedBox()
{
    if(rotatedBoxReady)
        return;
    float projection;

    VECTOR2 rotatedX(cos(spriteData.angle), sin(spriteData.angle));
    VECTOR2 rotatedY(-sin(spriteData.angle), cos(spriteData.angle));

    const VECTOR2 *center = getCenter();
    corners[0] = *center + rotatedX * ((float)edge.left*getScale())  +
                           rotatedY * ((float)edge.top*getScale());
    corners[1] = *center + rotatedX * ((float)edge.right*getScale()) + 
                           rotatedY * ((float)edge.top*getScale());
    corners[2] = *center + rotatedX * ((float)edge.right*getScale()) + 
                           rotatedY * ((float)edge.bottom*getScale());
    corners[3] = *center + rotatedX * ((float)edge.left*getScale())  +
                           rotatedY * ((float)edge.bottom*getScale());

    // corners[0] is used as origin
    // The two edges connected to corners[0] are used as the projection lines
    edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
    graphics->Vector2Normalize(&edge01);
    edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
    graphics->Vector2Normalize(&edge03);

    // this entities min and max projection onto edges
    projection = graphics->Vector2Dot(&edge01, &corners[0]);
    entA01min = projection;
    entA01max = projection;
    // project onto edge01
    projection = graphics->Vector2Dot(&edge01, &corners[1]);
    if (projection < entA01min)
        entA01min = projection;
    else if (projection > entA01max)
        entA01max = projection;
    // project onto edge03
    projection = graphics->Vector2Dot(&edge03, &corners[0]);
    entA03min = projection;
    entA03max = projection;
    projection = graphics->Vector2Dot(&edge03, &corners[3]);
    if (projection < entA03min)
        entA03min = projection;
    else if (projection > entA03max)
        entA03max = projection;

    rotatedBoxReady = true;
}

//=============================================================================
// Pixel Perfect collision detection method
// Called by collision()
// If the graphics card does not support a stencil buffer then CIRCLE
// collision is used.
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collidePixelPerfect(Entity &ent, VECTOR2 &collisionVector)
{
    if(graphics->getStencilSupport() == false)  // if stencil not supported
        return (collideCircle(ent, collisionVector));   // use CIRCLE collision

    // get fresh texture because they may have been released
    ent.spriteData.texture = ent.textureManager->getTexture();
    spriteData.texture = textureManager->getTexture();

    // if pixels are colliding
    pixelsColliding = graphics->pixelCollision(ent.getSpriteData(), this->getSpriteData());
    if(pixelsColliding > 0)
    {
        // set collision vector to center of entity
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// Is this Entity outside the specified rectangle
// Post: returns true if outside rect, false otherwise
//=============================================================================
bool Entity::outsideRect(RECT rect)
{
    if( spriteData.x + spriteData.width*getScale() < rect.left || 
        spriteData.x > rect.right ||
        spriteData.y + spriteData.height*getScale() < rect.top || 
        spriteData.y > rect.bottom)
        return true;
    return false;
}

//=============================================================================
// damage
// This entity has been damaged by a weapon.
// Override this function in the inheriting class.
//=============================================================================
void Entity::damage(int weapon)
{}

//=============================================================================
// Entity bounces after collision with another entity
//=============================================================================
void Entity::bounce(VECTOR2 &collisionVector, Entity &ent)
{
    float cUVdotVdiff;
    VECTOR2 Vdiff = ent.getVelocity() - velocity;
    VECTOR2 cUV = collisionVector;              // collision unit vector
    Graphics::Vector2Normalize(&cUV);
    if(collisionType == entityNS::ROTATED_BOX)  // if ROTATED_BOX collision
        // The collision vector is perpendicular to the edge. 
        cUVdotVdiff = 1.0f;                 // do not change velocity
    else
        cUVdotVdiff = Graphics::Vector2Dot(&cUV, &Vdiff);   // use velocity difference
    float massRatio = 2.0f;
    if (getMass() != 0)
        massRatio *= (ent.getMass() / (getMass() + ent.getMass()));
    if(massRatio < 0.1f)
        massRatio = 0.1f;

    // Move entities out of collision along collision vector
    VECTOR2 cv;
    int count=10;   // loop limit
    do
    {
        setX(getX() - cUV.x);
        setY(getY() - cUV.y);
        rotatedBoxReady = false;
        count--;
    } while( this->collidesWith(ent, cv) && count);

    // bounce
    deltaV += ((massRatio * cUVdotVdiff) * cUV);
}

//=============================================================================
// Force of gravity on this entity from other entity
// Adds the gravitational force to the velocity vector of this entity
// force = GRAVITY * m1 * m2 / r*r
//                    2              2
//  r*r  =   (Ax - Bx)   +  (Ay - By)
//=============================================================================
void Entity::gravityForce(Entity *ent, float frameTime)
{
    // if either entity is not active then no gravity effect
    if (!active || !ent->getActive())
        return ;

    rr = pow((ent->getCenterX() - getCenterX()),2) + 
            pow((ent->getCenterY() - getCenterY()),2);
    force = gravity * ent->getMass() * mass/rr;

    // --- Using vector math to create gravity vector ---
    // Create vector between entities
    VECTOR2 gravityV(ent->getCenterX() - getCenterX(),
                        ent->getCenterY() - getCenterY());
    // Normalize the vector
    Graphics::Vector2Normalize(&gravityV);
    // Multipy by force of gravity to create gravity vector
    gravityV *= force * frameTime;
    // Add gravity vector to moving velocity vector to change direction
    velocity += gravityV;
}
