#include <iostream>

#include "thread_queue.h"
#include <memory.h>
#include <thread>
#include <chrono>

#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#include <boost/filesystem.hpp>
#include <boost/range.hpp>

using namespace std;

void parse_directory(const std::shared_ptr<ThreadSafeQueue<std::string>>& queue, const std::string& dir, const std::string& file_ext){
    using namespace boost::filesystem;

    path p(dir);
    std::queue<path> directories;
    directories.push(p);

    while(!directories.empty())
    {
        auto url = directories.front();
        directories.pop();

        for(auto& entry : boost::make_iterator_range(directory_iterator(url), {}))
        {
            if(is_directory(entry)) {
                directories.push(entry);
            }
            else if(is_regular_file(entry) && entry.path().extension() == path(file_ext)){
                queue->push( entry.path().string());

            }
        }
    }


}

void create_caches(const std::shared_ptr<ThreadSafeQueue<std::string>>& queue, const std::string& dir_cache){

    using namespace boost::filesystem;
    do
    {
        if(queue->try_to_pop())
        {
            std::string value = queue->front();
            queue->pop();

            boost::filesystem::path p = boost::filesystem::path(dir_cache) /
                    boost::filesystem::path(value).parent_path();
            if(!boost::filesystem::exists(p)){
                boost::filesystem::create_directories(p);
            }

            const auto p1 = std::chrono::system_clock::now();

            auto rt = std::to_string(
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            p1.time_since_epoch()).count()) + ".jpg";

            auto write_path = p / boost::filesystem::path(rt);
            std::cout << "input " << write_path<<std::endl;

            cv::Mat img = cv::imread(value);
            cv::resize(img,img, cv::Size(100,100));
            cv::imwrite(write_path.string(), img);

        }
    }while(true);

}

// acync read image and create cache
/*
1. get files in directoru
2. queue to load
3.

*/
int main()
{

    std::string directory = "";
    std::string dir_cache = "";


    auto safe_q = std::make_shared<ThreadSafeQueue<std::string>>();
    //    parse_directory(safe_q, directory, ".jpg");
    std::thread th_parse(&parse_directory, safe_q, directory, ".jpg");

    //create thread pool
    std::thread th_create_cache(&create_caches,safe_q, dir_cache);
    std::thread th_create_cache2(&create_caches,safe_q, dir_cache);
    std::thread th_create_cache3(&create_caches,safe_q, dir_cache);
    std::thread th_create_cache4(&create_caches,safe_q, dir_cache);
    std::thread th_create_cache5(&create_caches,safe_q, dir_cache);
    std::thread th_create_cache6(&create_caches,safe_q, dir_cache);
//    std::thread th_create_cache7(&create_caches,safe_q, dir_cache);
//    std::thread th_create_cache8(&create_caches,safe_q, dir_cache);

    th_parse.join();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    th_create_cache.join();
    th_create_cache2.join();
    th_create_cache3.join();
    th_create_cache4.join();
    th_create_cache5.join();
    th_create_cache6.join();
//    th_create_cache7.join();
//    th_create_cache8.join();


    return 0;
}
