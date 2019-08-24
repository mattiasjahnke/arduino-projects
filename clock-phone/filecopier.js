const fs = require("fs");
const { execSync } = require("child_process");

const sourceFolderPath = "/Volumes/Externa/Engineerish/ClockPhone/clock-voice";
const destinationFolderPath = "/Volumes/NO NAME";

// Source file => Target file
const fileMappings = [
  ["current-time-is", "001"],
  ["minutes", "002"],
  ["past", "003"],
  ["0", "004"],
  ["1", "005"],
  ["2", "006"],
  ["3", "007"],
  ["4", "008"],
  ["5", "009"],
  ["6", "010"],
  ["7", "011"],
  ["8", "012"],
  ["9", "013"],
  ["10", "014"],
  ["11", "015"],
  ["12", "016"],
  ["13", "017"],
  ["14", "018"],
  ["15", "019"],
  ["16", "020"],
  ["17", "021"],
  ["18", "022"],
  ["19", "023"],
  ["20", "024"],
  ["30", "025"],
  ["40", "026"],
  ["50", "027"]
];

console.log(fileMappings[0][0]);

for (var i = 0; i < fileMappings.length; i++) {
  var mapping = fileMappings[i];
  const source = sourceFolderPath + "/" + mapping[0] + ".mp3";
  const destination = destinationFolderPath + "/" + mapping[1] + ".mp3";
  fs.copyFileSync(source, destination);
  console.log(i + ") " + source + " => " + destination);
}

let output = execSync("dot_clean '" + destinationFolderPath + "'");







