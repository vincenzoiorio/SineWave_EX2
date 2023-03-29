#include "ros/ros.h" 
#include "sinus/mymessage.h"
using namespace std;

int main(int argc, char** argv){

ros::init(argc, argv, "sinus_publisher_node");
ros::NodeHandle nh;
 // Crea un oggetto Publisher per pubblicare messaggi sul topic personalizzato
  ros::Publisher pub = nh.advertise<sinus::mymessage>("sinus_topic", 10);

  // Imposta la frequenza di pubblicazione in Hz
  ros::Rate loop_rate(10);

  // Variabili per la sinusoide
  double amplitude = 0.0;
  double period = 0.0;
  double frequency = 0.0;
  double phase_shift = 0.0;
  double time = 0.0;
  double dt = 0.1;

  //Ampiezza e Periodo
  cout << "Inserisci l'ampiezza della sinusoide: ";
  cin >> amplitude;
  cout << "Inserisci il periodo della sinusoide: ";
  cin >> period;

  // Calcola la frequenza della sinusoide
  frequency = 1.0 / period;

  // Ciclo di pubblicazione del messaggio
  while (ros::ok())
  {
    // Crea un nuovo messaggio
    sinus::mymessage msg;

    // Calcola il valore della sinusoide al tempo corrente
    double value = amplitude * sin(2 * M_PI * frequency * time + phase_shift);

    // Imposta i campi del messaggio
    msg.amplitude = amplitude;
    msg.period = period;
    if(abs(value)<1e-10){
        double valuezero=0;
        msg.value=valuezero;
        cout<<valuezero<<endl;
    }
    else{
    msg.value = value;
    cout<<value<<endl;
    }

    // Pubblica il messaggio sul topic personalizzato
    pub.publish(msg);

    // Attendi la prossima iterazione
    loop_rate.sleep();

    // Aggiorna il tempo
    time += dt;
  }

  return 0;
}