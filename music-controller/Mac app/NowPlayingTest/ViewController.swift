//
//  ViewController.swift
//  SerialController
//
//  Created by Mattias Jähnke on 2017-06-10.
//  Copyright © 2017 Mattias Jähnke. All rights reserved.
//

import Cocoa
import ORSSerial

class ViewController: NSViewController {

    fileprivate var serialPort: ORSSerialPort?
    var controller: MusicController!
    @IBOutlet weak var statusLabel: NSTextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        statusLabel.stringValue = "No connection"
        
        // TODO: Make sure to select the correct serial port!
        self.serialPort = ORSSerialPortManager.shared().availablePorts[0]
        self.serialPort!.baudRate = NSNumber(value: 9600)
        
        self.serialPort!.delegate = self
        self.serialPort!.open()

        controller = MusicController(changed: send)
    }
    
    fileprivate func send(track: Track?) {
        self.serialPort?.send(nowPlayingString(for: track).data(using: .ascii)!)
    }

    @IBAction func prev(_ sender: Any) {
        controller.previousTrack()
    }
    
    @IBAction func toggle(_ sender: Any) {
        controller.togglePlayPause()
    }
    
    @IBAction func next(_ sender: Any) {
        controller.nextTrack()
    }
}

extension ViewController: ORSSerialPortDelegate {
    func serialPortWasRemoved(fromSystem serialPort: ORSSerialPort) {
        self.serialPort = nil
    }
    
    func serialPort(_ serialPort: ORSSerialPort, didReceive data: Data) {
        if data.first == 0x56 {
            print("Vol: \(data[1])")
            MusicController.systemVolume = Int(data[1])
            return
        }
        guard let text = String(data: data, encoding: .ascii) else { return }
        switch text {
        case "T": controller.togglePlayPause()
        case "N": controller.nextTrack()
        case "P": controller.previousTrack()
        default: break
        }
        print("Recieved: \(text)")
    }
    
    func serialPort(_ serialPort: ORSSerialPort, didEncounterError error: Error) {
        print("Serial port (\(serialPort)) encountered error: \(error.localizedDescription)")
    }
    
    func serialPortWasOpened(_ serialPort: ORSSerialPort) {
        print("Serial port \(serialPort) was opened")
        statusLabel.stringValue = "Connected to:\n\(serialPort.path)"
        DispatchQueue.main.async {
            self.send(track: self.controller.currentTrack)
        }
    }
    
    func serialPortWasClosed(_ serialPort: ORSSerialPort) {
        print("Serial port was closed")
    }
}

func nowPlayingString(for track: Track?) -> String {
    return track.map { $0.artist + "\n" + $0.song } ?? "Not playing"
}
