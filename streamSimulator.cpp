#include "streamSimulator.hpp"

streamSimulator::streamSimulator(std::string imageFolder) :
    my_imagesFolder( imageFolder ), my_filetype(".jpg"),
    my_imageMutex( true ), my_synchroTime_ums( 100000 )
{
    DIR *dpdf;
    struct dirent *epdf;

    dpdf = opendir( (std::string("./") + imageFolder ).c_str() );
    if( dpdf != NULL )
    {
        std::string temp_fileName;
        while( (epdf = readdir(dpdf)))
        {
            temp_fileName = epdf->d_name;
            if( temp_fileName.find( my_filetype ) != std::string::npos )
                my_imagesVector.push_back( temp_fileName );
        }
    }

    std::sort( my_imagesVector.begin(), my_imagesVector.end() );

//    for(std::vector<std::string>::iterator i = my_imagesVector.begin();
//    i != my_imagesVector.end(); ++i)
//        std::cout<<(*i)<<std::endl;

    int thread_status = pthread_create( &my_simulatorThread, NULL,
                                        &streamSimulator::simulatorThreadStarter,
                                        (void*) this );
    if( thread_status == 0 )
        std::cout<<"Started stream simulator thread"<<std::endl;
}

void* streamSimulator::simulatorThreadStarter(void* class_instance)
{
    ((streamSimulator*) class_instance)->simulatorThread();
    return NULL;
}

void streamSimulator::simulatorThread()
{
    FILE* read_image;
    for(std::vector<std::string>::iterator i = my_imagesVector.begin();
            true; ++i)
    {
        if( i == my_imagesVector.end() )
        {
            i = my_imagesVector.begin() - 1;
            continue;
        }
//        std::cout<<(my_imagesFolder + "/" + (*i))<<std::endl;
        read_image = fopen((my_imagesFolder + "/" + (*i)).c_str(), "rb");
        if (read_image == NULL)
        {
            std::cout<<"Could not load: "<<(*i)<<std::endl;
            return;
        }

        fseek(read_image, 0, SEEK_END);
        fseek(read_image, 0, SEEK_END);
        my_imageDataLen = ftell(read_image);
        fseek(read_image, 0, SEEK_SET);

        my_imageMutex = true;
        my_imageData = fread(my_image, 1, my_imageDataLen, read_image);
        my_imageMutex = false;

        fclose(read_image);

        usleep( my_synchroTime_ums );
    }
}

void streamSimulator::waitForThreadExecution()
{
    pthread_join(my_simulatorThread, NULL);
}

int streamSimulator::getSynchroTime_ums()
{
    return my_synchroTime_ums;
}

std::pair<unsigned char*, int> streamSimulator::getImage() {
    while( my_imageMutex )
        usleep( 10 );
    return std::make_pair( &my_image[0], my_imageDataLen);
}
