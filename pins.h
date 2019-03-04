// Input
#define MOD_TX  2 // TxD - Must use a an interrupt pin
#define MOD_HS1 4 
#define MOD_HS2 3 

//optional
#define BUZZER 5
#define STATUS_LED 13 // This maps to the default onboard led for Arduino boards 

//unused? TX pin for LogicData class.
#define INTF_TX 10 // not used but needed for LogicData initialization - put any pin here

// 7 pin din socket
//1: RxD
//2: HS3
//3: HS1 up
//4: HS4
//5: HS2 down
//6: TxD
//7: +5V
//shell: Ground
