#include "HttpClient/HttpClient.h"

unsigned int nextTime = 0;

// ---- HTTP Variables ----
HttpClient http;

http_request_t request;
http_response_t response;

http_header_t headers[] = {
    { "Accept" , "*/*"},
    { NULL, NULL }
};

// ---- Memory ----
int lastFollowerCount = 0;
int lastLikeCount = 0;

char publishString[40]; // Used for debugging to the Particle console

void setup() {
    // Setup the endpoint for the Node.js server
    request.hostname = ""; // TODO: Insert hostname of your endpoint here
    request.port = 80;
    
    // (PWM) Pins for RGB Led
    pinMode(D1, OUTPUT); // Blue
    pinMode(D2, OUTPUT); // Green
    pinMode(D3, OUTPUT); // Red
}

void loop() {
    if (nextTime > millis()) {
        return;
    }
    
    int followerCount = getFollowers();
    int followerDelta = followerCount - lastFollowerCount;
    
    int likeCount = getLikes();
    int likeDelta = likeCount - lastLikeCount;
    
    // Skip the first run
    if (lastFollowerCount > 0 && lastLikeCount > 0) {
        handleResult(followerDelta, likeDelta);
    }
    
    sprintf(publishString, "F: %d, L: = %d", followerCount, likeCount);
    Spark.publish("InstagramStats", publishString);

    lastLikeCount = likeCount;
    lastFollowerCount = followerCount;

    nextTime = millis() + 10000;
}

void handleResult(int followerDelta, int likeDelta) {
    setColor(0, 0, 0); // Turn of LED
    
    // Favor "Followers" over "Likes"
    if (followerDelta > 0) {
        setColor(0, 0, 255);
    } else if (likeDelta > 0) {
        setColor(255, 0, 0);
    }
}

void setColor(int red, int green, int blue) {
  analogWrite(D3, red);
  analogWrite(D2, green);
  analogWrite(D1, blue);  
}

// ---- HTTP functions ----

int getFollowers() {
    request.path = "/followers/engineerish";
    return fetchInteger();
}

int getLikes() {
    request.path = "/likes/engineerish";
    return fetchInteger();
}

int fetchInteger() {
    http.get(request, response, headers);
    char* count = new char[response.body.length() + 1];
    strcpy(count, response.body.c_str());
    return atoi(count);
}