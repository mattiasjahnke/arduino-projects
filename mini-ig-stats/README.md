# mini-ig-stats

An RGB led that indicates whenever you get a new follower, comment or like on your Instagram account.

## How it works

### Node.js server

The server listens for http connections on port 3000 (locally).

Two routes are configured:

* `/followers/[instagram_handle]`
* `/likes/[instagram_handle]`
* `/comments/[instagram_handle]`

The server will cache the result of the Instagram queries and return the result as plain text (no json-wrapping BS) to the calling client.

### Photon

The photon will call the Node.js server every x second, asking for the current number of followers/comments/likes.

A simple in-memory cache of the latest result allows us to calculate the delta for the two values.
Therefore we can know when something has changed.

## Installation

This guide assumes you are up and running with `nodejs` and `npm`

### Node.js server

**1) Install dependencies**

`npm install`

**2) Run the project:**

`node index.js`

This will start the node project on your local machine and will listen for http connections on port `3000`

*Note: I recommend hosting the node application somewhere like Heroku*

### Photon

Visit [https://build.particle.io](https://build.particle.io) and upload the code to your Photon

**Modify the code**
* Configure your own hostname of the nodejs-application
* Replace my Instagram handle "engineerish" with your own
* Do any necessary modifications to the pin config (or write your own "result visualization" code)
