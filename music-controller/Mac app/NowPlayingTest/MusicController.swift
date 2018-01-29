//
//  MusicApps.swift
//  SerialController
//
//  Created by Mattias Jähnke on 2017-06-10.
//  Copyright © 2017 Mattias Jähnke. All rights reserved.
//

import Foundation

private let apps = [MusicApp("Spotify", notification: "com.spotify.client.PlaybackStateChanged"),
                    MusicApp("iTunes", notification: "com.apple.iTunes.playerInfo")]

class MusicController {
    private var listeners = [NSObjectProtocol]()
    private var changed: (Track?) -> ()
    var currentTrack: Track? {
        for app in apps {
            if let components = app.nowPlaying?.components(separatedBy: ":€:"), components.count == 2 {
                return Track(artist: components[0], song: components[1])
            }
        }
        return nil
    }
    
    private var app: MusicApp? {
        return apps.first { $0.isOpen }
    }
    
    static var systemVolume: Int {
        get { return 0 }
        set { "set volume output volume (\(newValue))".execute() }
    }
    
    init(changed: @escaping (Track?) -> ()) {
        self.changed = changed
        for name in apps.map({ $0.notification }) {
            let o = DistributedNotificationCenter.default.addObserver(forName: name, object: nil, queue: nil) { _ in
                let track = self.currentTrack
                print(nowPlayingString(for: track))
                self.changed(track)
            }
            listeners.append(o)
        }
    }
    
    func togglePlayPause() {
        app?.togglePlayPause()
    }
    
    func nextTrack() {
        app?.playNext()
    }
    
    func previousTrack() {
        app?.playPrevious()
    }
    
    deinit {
        for listener in listeners {
            DistributedNotificationCenter.default.removeObserver(listener)
        }
    }
}

struct Track {
    let artist: String
    let song: String
}

private struct MusicApp {
    let applicationName: String
    let notification: NSNotification.Name
    
    init(_ applicationName: String, notification: String) {
        self.applicationName = applicationName
        self.notification = NSNotification.Name(rawValue: notification)
    }
    
    var nowPlaying: String? {
        let script = """
            if application "\(applicationName)\" is running then
            tell application "\(applicationName)"
            if player state is playing then
            return (get artist of current track) & ":€:" & (get name of current track)
            else
            return ""
            end if
            end tell
            else
            return ""
            end if
            """
        
        return script.execute().stringValue.flatMap { $0.isEmpty ? nil : $0 }
    }
    
    var isOpen: Bool {
        return "return application \"\(applicationName)\" is running".execute().booleanValue
    }
    
    func togglePlayPause() {
        "tell application \"\(applicationName)\" to playpause".execute()
    }
    
    func playNext() {
        "tell application \"\(applicationName)\" to play next track".execute()
    }
    
    func playPrevious() {
        "tell application \"\(applicationName)\" to play previous track".execute()
    }
}

private extension String {
    @discardableResult
    func execute() -> NSAppleEventDescriptor {
        return NSAppleScript(source: self)!.executeAndReturnError(nil)
    }
}

