using System;
using Google.Apis.Discovery.v1;
using Google.Apis.Discovery.v1.Data;
using Google.Apis.Services;

//namespace Discovery.ListAPIs
namespace GoogleAPIsListing
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("Discovery API Sample");
            Console.WriteLine("====================");
            try
            {
                //                new Program().Run().Wait();
                new Program().Run();
            }
            catch (AggregateException ex)
            {
                foreach (var e in ex.InnerExceptions)
                {
                    Console.WriteLine("ERROR: " + e.Message);
                }
            }
            Console.WriteLine("Press any key to continue...");
            Console.ReadKey();
        }

        //        private async Task Run()
        private void Run()
        {
            // Create the service.
            var service = new DiscoveryService(new BaseClientService.Initializer
            {
                ApplicationName = "Discovery Sample"
                //,
                //                    APIKey = "[API Key]", //今回は必要ない
            });

            // Run the request.
            Console.WriteLine("Executing a list request...");
            //            var result = await service.Apis.List().ExecuteAsync();
            var result = service.Apis.List().ExecuteAsync();

            // Display the results.
            if (result.Result.Items != null)
            {
                foreach (DirectoryList.ItemsData api in result.Result.Items)
                {
                    Console.WriteLine(api.Id + " - " + api.Title);
                }
            }
        }
    }
}