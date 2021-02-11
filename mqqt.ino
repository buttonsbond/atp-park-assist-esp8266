//char homeassistanttopic[120];
char batterytopic[18];
char fsensortopic[18];
char ssensortopic[18];
//String(nochange)
char nochangetopic[18];
char fsettopic[18];
char ssettopic[18];
char curhrtopic[18];
//char ledbrighttopic[20];
char value[5]; 

#ifdef ENABLE_MQTT

// char mqtt_intopic[strlen(host) + 4 + 5];      // Topic in will be: <host>/in
// char mqtt_outtopic[strlen(host) + 5 + 5];     // Topic out will be: <host>/out
#define mqtt_server "192.168.1.131"
//strncpy(MQTTHOST,host,20);
#define mqtt_user "parking1"
#define mqtt_password "parking1"
#define mqtt_port 1883




#endif // of ifdef enable_mqtt

void mqtt_setup(){

#ifdef ENABLE_MQTT

client.setServer(mqtt_server, mqtt_port); // this established connection
client.setCallback(mqttcallback); // this tells where to send us if we get a message
//sprintf(homeassistanttopic,"%s/%s/%s/%s","homeassistant","sensor",host,"config");
sprintf(batterytopic, "%s/%s", host, "battery");
sprintf(fsensortopic, "%s/%s", host, "fsensor");
sprintf(ssensortopic, "%s/%s", host, "ssensor");
sprintf(nochangetopic, "%s/%s", host, "nochange");
sprintf(fsettopic, "%s/%s", host, "fset");
sprintf(ssettopic, "%s/%s", host, "sset");
sprintf(curhrtopic, "%s/%s", host, "curhour");
//sprintf(ledbrighttopic, "%s/%s", host, "ledbrightness");
mqttconnect();
mqttpublish();
//mqttsubscribe();

#endif
}

void mqttconnect(){
#ifdef ENABLE_MQTT
while (!client.connected()) {
Serial.println("Connecting to MQTT...");
client.connect(host, mqtt_user, mqtt_password);
if (client.connect(host, mqtt_user, mqtt_password )) {
Serial.println("mqtt connected");
}
} 
#endif
}

void mqttpublish(){
#ifdef ENABLE_MQTT  
  //String tt = String(selectedeffectno);
  //sprintf(value,"%u",selectedeffectno);
  //client.publish(effecttopic,value,false);
  //client.publish(startuptopic,(autoeffects ==1)? "1": "0",false);
  //client.publish(cycletopic,(randomeffects == 1)? "1": "0",false);
  //client.publish(offtopic,(autoeffects ==1)? "1": "0",false);
  // i observed 588 to be fully charged on my d1 mini with the resistors I used for the voltage divider
  //float voltage = analogRead(VOLTAGE_IN) * (3.7 / 588);
  char tempString[8];
  GetVoltage();
  //String v = String(voltage,4);
  sprintf(value,"%c",mytime);
  //dtostrf(CurrentHour,1,2,tempString);
  client.publish(curhrtopic,mytime,false);
  dtostrf(voltage, 1, 2, tempString);
  client.publish(batterytopic,tempString);
  //      client.publish(curhrtopic,tempString);
  sprintf(value,"%u",distanceSide);
  client.publish(ssensortopic,value,false);
  sprintf(value,"%u",distanceFront);
  client.publish(fsensortopic,value,false);  
  sprintf(value,"%u",nochange);
  client.publish(nochangetopic,value,false);
  sprintf(value,"%u",SetDistanceFront);
  client.publish(fsettopic,value,false);
  sprintf(value,"%u",SetDistanceSide);
  client.publish(ssettopic,value,false);
 // dtostrf(ledbrightness,1,2,tempString);
//  client.publish(ledbrighttopic,tempString,false);
  #ifdef SERIAL_DEBUG
    Serial.print(mytime);
    Serial.println(" - published mqtt");
  #endif
#endif
}
void mqttsubscribe(){
#ifdef ENABLE_MQTT
// don't think we need to subscribe to anything unless you want the sketch to
// respond to mqtt messages
//   client.subscribe(batterytopic);
//  client.subscribe(effecttopic);
//  client.subscribe(startuptopic);
//  client.subscribe(cycletopic);
//  client.subscribe(offtopic);
#endif
}

void mqttcallback(String topic, byte* payload, unsigned int length) {
#ifdef ENABLE_MQTT
  //
  // to implement
  // subscribe to fsettopic and ssettopic to allow changing settings via MQTT if needed
  // 
  String msg = "";
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("payload length was: ");
  Serial.println(length);
  payload[length] = '\0';
  msg = (char*)payload;
  Serial.print("Message:");
  Serial.println(msg);

#endif
}
