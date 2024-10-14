
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <math.h>


#define yikamaMotoruRole 16
#define kirliSuMotoruRole 5
#define deterjanKapakRole 4
#define isiticiMotoruRole 0
#define temizSuMotoruRole 2
#define buzzerPin 15

#define NormalCalistirButon 12

#define mesajBombasi 13
   //bayrak xler,i değiştir*********************---------------------****************************-------------------------*******************************--------------------------**************-*-*-*-*-*-*-*-*-*-*-*-*-**
// Initialize Wifi connection to the router
char ssid[] = "xxx";     // your network SSID (name)
char password[] = "xxx"; // your network key



// Notaların frekansları (C4'ten C5'e kadar notalar)
int notalar[] = {
  262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880  // Temel notalar
};

// Death by Glamour için notalar ve süreler
int deathByGlamourMelody[] = {
  659, 659, 659, 523, 659, 784, 392, 659, 659, 784, 784, 659
};

int deathByGlamourDurations[] = {
  250, 250, 250, 250, 250, 500, 500, 250, 250, 500, 250, 1000  // Süreler milisaniye cinsinden
};

// Megalovania için notalar ve süreler
int megalovaniaMelody[] = {
  392, 392, 392, 311, 392, 466, 466, 392, 311, 392, 466, 392
};

int megalovaniaDurations[] = {
  250, 250, 500, 250, 250, 500, 500, 250, 250, 500, 250, 1000  // Süreler milisaniye cinsinden
};



   //bayrak xler,i değiştir*********************---------------------****************************-------------------------*******************************--------------------------**************-*-*-*-*-*-*-*-*-*-*-*-*-**
// Initialize Telegram BOT
#define BOTtoken "xxx:xxxxxxxx"  // your Bot Token (Get from Botfather)

#define BOTtoken2 "xxxx:xxxxxxx-3pFgPmfmQ"  // your Bot Token (Get from Botfather)

WiFiClientSecure client; // bot1 için ayrı istemci
WiFiClientSecure client2; // bot2 için ayrı istemci

UniversalTelegramBot bot(BOTtoken, client);  // bot1 için
UniversalTelegramBot bot2(BOTtoken2, client2);  // bot2 için


int Bot_mtbs = 2000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

// NTC özellikleri
const int NTC_PIN = A0;    // NTC sensörünün bağlı olduğu analog pin
const float SERIES_RESISTOR = 10000.0; // 10K seri direnç
const float NOMINAL_RESISTANCE = 10000.0; // 10K NTC'nin nominal direnci
const float NOMINAL_TEMPERATURE = 25.0;  // 25°C nominal sıcaklık
const float B_COEFFICIENT = 3950.0;      // NTC'nin beta katsayısı
const int ADC_MAX_VALUE = 1023;          // ADC'nin maksimum değeri

void checkForNewMessages() {
  //  Serial.print("check ");

  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  if (numNewMessages>0) {  //  Serial.print("checkooo ");

    handleNewMessages(numNewMessages);
  }
}

// Sıcaklık ölçme fonksiyonu
float sicaklikOlc() {
  int adcValue = analogRead(NTC_PIN);

  // Gerilim bölücü formülüyle NTC direncini hesapla
  float resistance = SERIES_RESISTOR / ((ADC_MAX_VALUE / (float)adcValue) - 1.0);

  // Steinhart-Hart denklemi ile sıcaklığı hesapla
  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;          // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= B_COEFFICIENT;                           // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);    // + (1/To)
  steinhart = 1.0 / steinhart;                          // T = 1 / {...}
  steinhart -= 273.15;                                  // Kelvin'den Celcius'a dönüştür

 
  return steinhart;  // Celsius olarak sıcaklık döndür



}

void mesajbombafonksiyonu(){

  // Butona basıldığında
  if (digitalRead( mesajBombasi )== LOW) {
   // Serial.println("Butona basıldı, mesaj gönderiliyor...");
    String CHAT_ID2 = "xxxxxxxx";      //bayrak xler,i değiştir*********************---------------------****************************-------------------------*******************************--------------------------**************-*-*-*-*-*-*-*-*-*-*-*-*-**
    // İlk mesajı gönder

    for(int i=0;i<10;i++){
    bot2.sendMessage(CHAT_ID2, "MERHABA Necva", "");
    
    // İkinci mesajı gönder
    bot2.sendMessage(CHAT_ID2, "Ben BULAŞIK MAKİNASI, YIKAYICI", "");

    }

    
    // Butona yeniden basmayı beklemek için ufak bir gecikme
    delay(1000);  // Buton tekrar basılana kadar kısa süre bekleme
    } }
void yikamaYapmaZamani(int sicaklikDegerriBudur){

        deathByGlamour();


        durulama(sicaklikDegerriBudur);
        deterjanliYikilama(sicaklikDegerriBudur);
        durulama(sicaklikDegerriBudur);


        megalovania(); //bitit müziği

        digitalWrite(buzzerPin, LOW);  // Isıtıcıyı kapat
}

void handleNewMessages(int numNewMessages) {
 // Serial.println("handleNewMessages");
//  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
//Serial.println("1bbb");

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 // Serial.println(from_name);

    if (text == "/gucluYikama") {
      bot.sendMessage(chat_id, "Güçlü Yıkama başlatılıyor", "");
      yikamaYapmaZamani(70);
      // Yıkama işlemleri buraya gelecek
    } 
    else if (text == "/sadeceDurulama") {
      bot.sendMessage(chat_id, "Sadece Durulama başlatılıyor", "");
      durulama(70);
      // Durulama işlemleri buraya gelecek
    } 
    else if (text == "/kapakAc") {
      bot.sendMessage(chat_id, "Kapak açılıyor", "");
      superIslem(40000, deterjanKapakRole);
      digitalWrite(deterjanKapakRole, LOW); 
    } 
    else if (text == "/durulamaAc") {
      bot.sendMessage(chat_id, "Durulama açılıyor", "");
      durulama(70);
    } 
    else if (text == "/durulamaKapat") {
      bot.sendMessage(chat_id, "Durulama durduruluyor", "");
      digitalWrite(yikamaMotoruRole, LOW);
      digitalWrite(kirliSuMotoruRole, LOW);
      digitalWrite(deterjanKapakRole, LOW);
      digitalWrite(isiticiMotoruRole, LOW);
      digitalWrite(temizSuMotoruRole, LOW);
      digitalWrite(buzzerPin, LOW);
    } 
    else if (text == "/suAlma") {
      bot.sendMessage(chat_id, "Su alınıyor", "");
      superIslem(60000, temizSuMotoruRole);
    } 
    else if (text == "/suAtma") {
      bot.sendMessage(chat_id, "Su boşaltılıyor", "");
      superIslem(120000, kirliSuMotoruRole);
    } 
    else if (text == "/40CYikama") {
      bot.sendMessage(chat_id, "40C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(40);
    } 
    else if (text == "/50CYikama") {
      bot.sendMessage(chat_id, "50C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(50);
    } 
    else if (text == "/60CYikama") {
      bot.sendMessage(chat_id, "60C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(60);
    } 
    else if (text == "/70CYikama") {
      bot.sendMessage(chat_id, "70C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(70);
    } 
    else if (text == "/80CYikama") {
      bot.sendMessage(chat_id, "80C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(80);
    } 
    else if (text == "/90CYikama") {
      bot.sendMessage(chat_id, "90C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(90);
    } 
    else if (text == "/100CYikama") {
      bot.sendMessage(chat_id, "100C Yıkama Başlatılıyor.", "");
      yikamaYapmaZamani(100);
    } 
    else if (text == "/megalovania") {
      bot.sendMessage(chat_id, "Megalovania çalınıyor", "");
      megalovania();
    } 
    else if (text == "/glamour") {
      bot.sendMessage(chat_id, "Glamour çalınıyor", "");
      deathByGlamour();
    } 
    else if (text == "/buzzer") {
      bot.sendMessage(chat_id, "Buzzer çalınıyor", "");
      digitalWrite(buzzerPin, HIGH);
      delay(1000);
      digitalWrite(buzzerPin, LOW);
    } 
    else if (text == "/durum") {
      String statusMessage = "Durum: \n";
      statusMessage += digitalRead(yikamaMotoruRole) ? "Yıkama Motoru açık\n" : "Yıkama Motoru kapalı\n";
      statusMessage += digitalRead(kirliSuMotoruRole) ? "Kirli Su Motoru açık\n" : "Kirli Su Motoru kapalı\n";    
      statusMessage += digitalRead(deterjanKapakRole) ? "Deterjan Kapak açık\n" : "Deterjan Kapak kapalı\n";    
      statusMessage += digitalRead(isiticiMotoruRole) ? "Isıtıcı Motoru açık\n" : "Isıtıcı Motoru kapalı\n";    
      statusMessage += digitalRead(temizSuMotoruRole) ? "Temiz Su Motoru açık\n" : "Temiz Su Motoru kapalı\n";    
      statusMessage += digitalRead(buzzerPin) ? "Buzzer açık\n" : "Buzzer kapalı\n";    
      bot.sendMessage(chat_id, statusMessage, "");
    }  else if (text == "/sicaklikOlc") {
      
        bot.sendMessage(chat_id, "Sıcaklık: " + String(sicaklikOlc()) + " C");

    }    else if (text == "/demo") {
      
        bot.sendMessage(chat_id, "DEMO CALISTIRILIYOR: ");

        
  digitalWrite(yikamaMotoruRole, LOW); 
  digitalWrite(kirliSuMotoruRole, LOW); 
  digitalWrite(deterjanKapakRole, LOW); 
  digitalWrite(isiticiMotoruRole, LOW); 
  digitalWrite(temizSuMotoruRole, LOW); 
  digitalWrite(buzzerPin, HIGH); 
  delay(3000);
  digitalWrite(yikamaMotoruRole, HIGH); 
  digitalWrite(kirliSuMotoruRole, HIGH); 
  digitalWrite(deterjanKapakRole, HIGH); 
  digitalWrite(isiticiMotoruRole, HIGH); 
  digitalWrite(temizSuMotoruRole, HIGH); 
  digitalWrite(buzzerPin, LOW); 

    demoPropgram();

    }  
    else if (text == "/start") {  //Serial.println("start");

      String welcome = "Merhaba " + from_name + ". Komutlar:\n";
      welcome += "/gucluYikama : Güçlü Yıkamayı başlat\n";
      welcome += "/sadeceDurulama : Sadece Durulamayı başlat\n";
      welcome += "/megalovania : megalovania çal\n";
      welcome += "/buzzer : Buzzer Aç başlat\n";
      welcome += "/glamour : Glamour çal\n";
      welcome += "/durum : Cihazların durumunu öğren\n";
         welcome += "/40CYikama : 40 C Yıkamayı başlat y\n";
      welcome += "/50CYikama : 50 C Yıkamayı başlat\n";
      welcome += "/60CYikama : 60 C Yıkamayı başlat\n";
      welcome += "/70CYikama : 70 C Yıkamayı başlat\n";
      welcome += "/80CYikama : 80 C Yıkamayı başlat\n";
      welcome += "/90CYikama : 90 C Yıkamayı başlat\n";
      welcome += "/100CYikama : 100 C Yıkamayı başlat\n";
        welcome += "/kapakAc : Kapak aç\n";
      welcome += "/durulamaAc : Durulama başlat\n";
      welcome += "/durulamaKapat : Durulamayı durdur\n";
      welcome += "/suAlma : Su al\n";
      welcome += "/suAtma : Su boşalt\n";
            welcome += "/sicaklikOlc : sicaklik Olcer\n";

       bot.sendMessage(chat_id, welcome, "Markdown");

    }   


    else {
      // Bilinmeyen komutlar için
      bot.sendMessage(chat_id, "Hatalı komut girdiniz. Lütfen geçerli bir komut girin. Komut listesi için /start yazabilirsiniz.", "");
    }
  }
}

void setup() {


  pinMode(yikamaMotoruRole, OUTPUT);
  pinMode(kirliSuMotoruRole, OUTPUT);
  pinMode(deterjanKapakRole, OUTPUT);
  pinMode(isiticiMotoruRole, OUTPUT);
  pinMode(temizSuMotoruRole, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(NormalCalistirButon, INPUT_PULLUP);  // Butonu giriş olarak ayarla ve pull-up direnci aktif et
  pinMode(mesajBombasi, INPUT_PULLUP);  // Dahili pull-up direnci kullanılıyor


  digitalWrite(yikamaMotoruRole, HIGH); 
  digitalWrite(kirliSuMotoruRole, HIGH); 
  digitalWrite(deterjanKapakRole, HIGH); 
  digitalWrite(isiticiMotoruRole, HIGH); 
  digitalWrite(temizSuMotoruRole, HIGH); 
  digitalWrite(buzzerPin, LOW); 


  client.setInsecure();
  client2.setInsecure();


  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

 // Serial.print("Connecting Wifi: ");
 // Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
 //   Serial.print(".");
    delay(500);
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.print("IP address: ");
 // Serial.println(WiFi.localIP());





}

void loop() {
   mesajbombafonksiyonu();
   delay(1000);


  digitalWrite(yikamaMotoruRole, HIGH); 
  digitalWrite(kirliSuMotoruRole, HIGH); 
  digitalWrite(deterjanKapakRole, HIGH); 
  digitalWrite(isiticiMotoruRole, HIGH); 
  digitalWrite(temizSuMotoruRole, HIGH); 
  digitalWrite(buzzerPin, LOW); 
   

  if (millis() > Bot_lasttime + Bot_mtbs)  {
    checkForNewMessages();  // Check for new messages at regular intervals
    Bot_lasttime = millis();
  //Serial.print("mılıs ");

  }

  if (digitalRead(NormalCalistirButon) == LOW) {  // Butona basıldığında LOW olur
    // Butona tıklandığında derceliYikama fonksiyonunu çalıştır
         yikamaYapmaZamani(70);
    delay(500);  // Butonun tekrar algılanmaması için kısa bir gecikme
  delay(1000);
}/*
*/
}


void superIslem(int milisaniye, int cikisPini) {
  unsigned long startMillis = millis(); // Başlangıç zamanı
  unsigned long currentMillis;

  while (millis() - startMillis < milisaniye) {
    currentMillis = millis();
    
    // Belirtilen pin ile motoru çalıştır
    digitalWrite(cikisPini, LOW);  // Örnek işlem: motoru çalıştır
    
    // Yeni mesajları kontrol et
    checkForNewMessages(); // Bu fonksiyonda bot.getUpdates() çağırarak yeni mesajları kontrol et

    // Kısa bir gecikme ile sürekli döngü devam ettiriliyor
    delay(1000);  // 100 ms kısa gecikme
  }

  // İşlem bitince motoru durdur
  digitalWrite(cikisPini, HIGH); 
}


void isiticiKontrol(int maxSicaklik, int maxminDegeri) {
  float currentTemperature = sicaklikOlc();

  static bool isiticiAcik = false;  // Isıtıcının açık olup olmadığını kontrol eden bayrak

  if (currentTemperature < maxSicaklik && !isiticiAcik) {
    // Eğer sıcaklık maksimum değerin altındaysa ve ısıtıcı kapalıysa ısıtıcıyı aç
    digitalWrite(isiticiMotoruRole, HIGH);  // Isıtıcıyı aç
    isiticiAcik = true;  // Isıtıcı açık
  } else if (currentTemperature >= maxSicaklik && isiticiAcik) {
    // Eğer sıcaklık maksimum değere ulaştıysa ve ısıtıcı açıksa ısıtıcıyı kapat
    digitalWrite(isiticiMotoruRole, LOW);  // Isıtıcıyı kapat
    isiticiAcik = false;  // Isıtıcı kapalı
  }

  // Isıtıcı kapandıktan sonra belirlenen aralığa kadar soğumasını bekle
  if (isiticiAcik == false && currentTemperature <= (maxSicaklik - maxminDegeri)) {
    // Isıtıcı tekrar açılabilir
    isiticiAcik = false;  // Isıtıcı tekrar açılmaya hazır
  }
}


void durulama(int sicaklikDegerri){

        superIslem(60000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye
        superIslem(60000, temizSuMotoruRole);      // Temiz su motoru 60 saniye
                  digitalWrite(temizSuMotoruRole, HIGH); 
        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        digitalWrite(isiticiMotoruRole, HIGH);  // Isıtıcıyı kapat
        superIslem(1300000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        superIslem(80000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye
        superIslem(10000, temizSuMotoruRole);      // Temiz su motoru 60 saniye
                  digitalWrite(temizSuMotoruRole, HIGH); 

        superIslem(40000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye

}






void deterjanliYikilama(int sicaklikDegerri){
  
 //ön yıkama
        superIslem(60000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye
        superIslem(60000, temizSuMotoruRole);      // Temiz su motoru 60 saniye
          digitalWrite(temizSuMotoruRole, HIGH); 

        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        superIslem(60000, deterjanKapakRole);      // Deterjan kapağı motoru 60 saniye

        isiticiKontrol(sicaklikDegerri, 10);      // Isıtıcı motor 60 saniye
        superIslem(120000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        
        digitalWrite(isiticiMotoruRole, HIGH);  // Isıtıcıyı kapat
        superIslem(2000000, yikamaMotoruRole);       // Yıkama motoru 60 saniye

        superIslem(80000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye
        superIslem(10000, temizSuMotoruRole);      // Temiz su motoru 60 saniye
                  digitalWrite(temizSuMotoruRole, HIGH); 

        superIslem(40000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye

}




void deathByGlamour() {
  // Death by Glamour melodisi
  for (int i = 0; i < sizeof(deathByGlamourMelody) / sizeof(deathByGlamourMelody[0]); i++) {
    tone(buzzerPin, deathByGlamourMelody[i]);    // Notaları çal
    delay(deathByGlamourDurations[i]);           // Süre kadar bekle
    noTone(buzzerPin);                           // Sesi durdur
    delay(50);                                   // Kısa bir duraklama
  }
}

void megalovania() {





  // Megalovania melodisi
  for (int i = 0; i < sizeof(megalovaniaMelody) / sizeof(megalovaniaMelody[0]); i++) {
    tone(buzzerPin, megalovaniaMelody[i]);       // Notaları çal
    delay(megalovaniaDurations[i]);              // Süre kadar bekle
    noTone(buzzerPin);                           // Sesi durdur
    delay(50);                                   // Kısa bir duraklama
  }

  }




void demoPropgram(){

     superIslem(5000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye
        superIslem(5000, temizSuMotoruRole);      // Temiz su motoru 60 saniye
        delay(5000);
        digitalWrite(isiticiMotoruRole, LOW);         
       delay(5000);
        digitalWrite(isiticiMotoruRole, HIGH);  // Isıtıcıyı kapat

        superIslem(5000, yikamaMotoruRole);       // Yıkama motoru 60 saniye
        superIslem(5000, deterjanKapakRole);      // Deterjan kapağı motoru 60 saniye

        
        digitalWrite(isiticiMotoruRole, HIGH);  // Isıtıcıyı kapat
        superIslem(5000, yikamaMotoruRole);       // Yıkama motoru 60 saniye

        superIslem(5000, kirliSuMotoruRole);      // Kirli su motoru 60 saniye









}
































