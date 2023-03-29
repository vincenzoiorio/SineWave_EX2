#include "ros/ros.h" 
#include "std_msgs/Float64.h"
#include "sinus/mymessage.h"
#include <boost/thread.hpp>

using namespace std;

class SinusSubscriber {
  
    public:
        SinusSubscriber();
        void topic_cb(const sinus::mymessage::ConstPtr& m);
        void filter_and_publish();
  
    private:

    ros::NodeHandle _nh;
    ros::Subscriber _sub;
    ros::Publisher _pub; 
    boost::thread _thread;
    double _value; // variabile membro per memorizzare il valore della sinusoide
};


SinusSubscriber::SinusSubscriber(){
    //Crea un oggetto Subscriber per ricevere 
    //i messaggi sul topic personalizzato
_sub = _nh.subscribe("sinus_topic", 10, &SinusSubscriber::topic_cb, this);
    // Crea un oggetto Publisher per pubblicare i dati filtrati
_pub = _nh.advertise<std_msgs::Float64>("filtered_topic", 10);

_thread = boost::thread(&SinusSubscriber::filter_and_publish, this);
}

void SinusSubscriber::topic_cb(const sinus::mymessage::ConstPtr& msg){

double amplitude = msg->amplitude;
double period = msg->period;
double value = msg->value;
//ROS_INFO("Listener: %f", msg->value);
_value=msg->value;
}
void SinusSubscriber::filter_and_publish() {
    // loop per pubblicazione periodica dei dati filtrati
    ros::Rate loop_rate(10); // pubblicazione ogni 100ms
    double alpha=0.5;
    double prev_value=0.0;
    while (ros::ok()) {
        // filtra il valore memorizzato con un filtro passa-basso
        
        double filtered_value =(1-alpha)*prev_value + alpha*_value; // esempio di filtro passa-basso
        prev_value=_value;
        std_msgs::Float64 filtered_msg;
        filtered_msg.data = filtered_value;

        // pubblica i dati filtrati
        _pub.publish(filtered_msg);

        ros::spinOnce();
        loop_rate.sleep();
    }
}





int main(int argc, char** argv){

ros::init(argc, argv, "sinus_subscriber_node");

SinusSubscriber subscriber;

ros::spin();

return 0;

}
