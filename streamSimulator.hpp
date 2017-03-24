#ifndef STREAMSIMULATOR_HPP_INCLUDED
#define STREAMSIMULATOR_HPP_INCLUDED

#include <pthread.h>
#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <cstdio>

class streamSimulator
{
public:
    streamSimulator( std::string imagesFolder = "video_stream" );
    static void* simulatorThreadStarter( void* class_instance );
    void waitForThreadExecution(void);
    int getSynchroTime_ums(void);
    std::pair<unsigned char*, size_t> getImage(void);

protected:
    void simulatorThread(void);
private:
    std::string my_imagesFolder;
    pthread_t my_simulatorThread;
    std::vector<std::string> my_imagesVector;
    std::string my_filetype;
    unsigned char my_image[1000000];
    size_t my_imageData;
    bool my_imageMutex;
    int my_synchroTime_ums;
};

#endif // STREAMSIMULATOR_HPP_INCLUDED
