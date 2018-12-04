//
// Created by anton on 26.11.18.
//
#include <thread>
#include "rabbitmq_worker.hpp"

namespace uos{


    void rabbitmq_worker::run() {
        if(rabbit_queue== nullptr){
            std::cout<<"Rabbit queue not initialized"<<std::endl;
            return;
        }
        rb_channel.consume(rb_queue_name, AMQP::noack).onReceived(
                [&](const AMQP::Message &message,
                    uint64_t deliveryTag,
                    bool redelivered)
                {
                    auto received_json = std::string(message.body(),message.bodySize());
                    std::cout<<" [x] Received "
                       << received_json
                       << std::endl;
                    if(received_json.size()>0){
                        rabbit_queue->push(std::string(message.body(),message.bodySize()));
                        std::cout<<rabbit_queue->size()<<" - "<<std::endl;
//                        if(rabbit_queue->size()>100000)
//                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                });
        std::cout << " [*] Waiting for messages. To exit press CTRL-C"<<std::endl;
        poco_handler.loop();

    }
    void rabbitmq_worker::stop() {
        std::cout<<"Rabbit quit"<<std::endl;
        poco_handler.quit();
    }

}
