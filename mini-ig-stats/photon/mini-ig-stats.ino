#include "HttpClient/HttpClient.h"

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

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
int lastCommentCount = 0;

char publishString[40]; // Used for debugging to the Particle console

void setup() {
    // Setup the endpoint for the Node.js server
    request.hostname = "";
    request.port = 80;
    
    // (PWM) Pins for RGB Led
    pinMode(D1, OUTPUT); // Red
    pinMode(D2, OUTPUT); // Blue
    pinMode(D3, OUTPUT); // Green
    
    // Test LEDs
    setColor(255, 0, 0);
    delay(2000);
    setColor(0, 255, 0);
    delay(2000);
    setColor(0, 0, 255);
    delay(2000);
    setColor(0, 0, 0);
}

void loop() {
    if (nextTime > millis()) {
        return;
    }
    
    int followerCount = getFollowers();
    int followerDelta = followerCount - lastFollowerCount;
    
    int likeCount = getLikes();
    int likeDelta = likeCount - lastLikeCount;
    
    int commentCount = getComments();
    int commentDelta = commentCount - lastCommentCount;
    
    // Skip the first run
    if (lastFollowerCount > 0 && lastLikeCount > 0 && lastFollowerCount > 0) {
        handleResult(followerDelta, likeDelta, commentDelta);
    }
    
    sprintf(publishString, "F: %d, L: = %d, C: %d", followerCount, likeCount, commentCount);
    Spark.publish("InstagramStats", publishString);

    lastLikeCount = likeCount;
    lastFollowerCount = followerCount;
    lastCommentCount = commentCount;

    nextTime = millis() + 10000;
}

void handleResult(int followerDelta, int likeDelta, int commentDelta) {
    setColor(0, 0, 0); // Turn off LED
    
    // Favor "Followers" over "Comments"
    if (followerDelta > 0) {
        setColor(0, 0, 255);
    } else if (commentDelta > 0) { // Favor "Comments" over "Likes"
        setColor(0, 255, 0);
    } else if (likeDelta > 0) {
        setColor(255, 0, 0);
    }
}

void setColor(int red, int green, int blue) {
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(D1, red);
  analogWrite(D3, green);
  analogWrite(D2, blue);  
}

// ---- HTTP functions ---- (Change the instagram handle)

int getFollowers() {
    request.path = "/followers/engineerish";
    return fetchInteger();
}

int getLikes() {
    request.path = "/likes/engineerish";
    return fetchInteger();
}

int getComments() {
    request.path = "/comments/engineerish";
    return fetchInteger();
}

int fetchInteger() {
    http.get(request, response, headers);
    char* count = new char[response.body.length() + 1];
    strcpy(count, response.body.c_str());
    return atoi(count);
}