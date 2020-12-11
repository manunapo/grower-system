#define TIME_ZONE -3 //NYC(USA): -5 UTC

#define LGTPIN 0    // GPIO 0 = D3
#define BMBPIN 4    // GPIO 4 = D2 
#define FANPIN 5    // GPIO 5 = D1

#define LD1PIN 12    // GPIO 12 = D6
#define LD2PIN 13    // GPIO 13 = D7 
#define LD3PIN 15    // GPIO 15 = D8

#define DHTPIN 14    // GPIO 14 = D5. Pin donde está conectado el sensor

#define GRDPIN 0    // ADC0 = A0

//#define DHTTYPE DHT11   // Descomentar si se usa el DHT 11
#define DHTTYPE DHT22   // Sensor DHT22

/*
  1 second = 1000 milliseconds
  1 minute = 60 seconds
  1 hour = 60 minutes
  12 hours = 12 * 60 * 60 * 1000 = 43,200,000
  15 minutes = 15 * 60 * 1000 = 900,000
*/
#define SAMPLING 900000UL
//#define SAMPLING 15000

#define WATERING 5000
