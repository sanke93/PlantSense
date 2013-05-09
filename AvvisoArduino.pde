import com.avvisoapp.Avviso;
import processing.serial.Serial;

Serial serial;
Avviso avviso;

void setup() {
  size(400,400);
  smooth();
  background(255);
  fill(0);
  textAlign(CENTER);
  text("Plantsense.", width/2, height * 0.8);

  PImage b;
  // Images must be in the "data" directory to load correctly
  b = loadImage("PlantSense.jpg");
  image(b, 0, 20);
  
  // Open the first serial device
  serial = new Serial(this, Serial.list()[0], 115200);
 
  avviso = new Avviso(Avviso.DeliveryMethod.NOTIFY_MY_ANDROID);
 
}

void draw() {
  if (serial.available() > 0) {
    String s = serial.readStringUntil(10);  // Read until a newline
    if (s != null) {
      if (s.startsWith(Avviso.AVVISO_API_KEY)) {
        s = s.substring(Avviso.AVVISO_API_KEY.length());
        s = s.trim();
        avviso.setApiKey(s);
      } else if (s.startsWith(Avviso.AVVISO_APPLICATION_NAME)) {
        s = s.substring(Avviso.AVVISO_APPLICATION_NAME.length());
        s = s.trim();
        avviso.setApplicationName(s);
      } else if (s.startsWith(Avviso.AVVISO_PUSH)) {
        String eventName = serial.readStringUntil(10);  // Read until a newline
        eventName = eventName.trim();
        String messageString = serial.readStringUntil(10);  // Read until a newline
        messageString = messageString.trim();
        String priorityString = serial.readStringUntil(10);  // Read until a newline
        priorityString = priorityString.trim();
        int priority = Integer.parseInt(priorityString);
        avviso.push(eventName, messageString, priority);
      } else {
        // If the line doesn't start with CIAO_PRINT or CIAO_PRINTLN,
        // it's a debug message. Print it to the screen.
        s = s.trim();
        println(s);
      }
    }
  }
}
