import processing.serial.*;

Serial myPort;
final int maxTTL = 120;

ArrayList<Reading> readings = new ArrayList<Reading>();

void setup() {
  strokeWeight(2);
  fullScreen();
  myPort = new Serial(this, Serial.list()[1], 9600);
}

void draw() {
  background(0);

  if (myPort.available() > 0) {
    String val = myPort.readStringUntil('\n');
    if (val != null) {
      String[] parts = val.trim().split("\\+");
      if (parts.length == 2) {
        int angle = parseInt(parts[0]);
        int distance = parseInt(parts[1]);
        readings.add(new Reading(angle, distance));
      }
    }
  }

  for (Reading reading : readings) {
    reading.show();
  }

  // Remove dead readings
  for (int i = readings.size() - 1; i >= 0; i--) {
    Reading reading = readings.get(i);
    if (reading.isDead()) {
      readings.remove(i);
    }
  }
}

class Reading {
  private int ttl = maxTTL;
  private int angle = 0;
  private int distance = 0;
  
  public Reading(int angle, int distance) {
    this.angle = angle;
    this.distance = max(min(distance, 100), 0);
  }
  
  void show() {
    ttl--;
    stroke(255, map(ttl, 0, maxTTL, 0, 100));
    lineAngle(width / 2, height, radians(angle), norm(distance, 0, 100) * height);
  }
  
  boolean isDead() {
    return ttl <= 0;
  }
  
  void lineAngle(int x, int y, float angle, float length) {
    line(x, y, x+cos(angle)*length, y-sin(angle)*length);
  }
}