//
//  ViewController.swift
//  PixelDrawer
//
//  Created by Mattias Jähnke on 2018-04-22.
//  Copyright © 2018 Engineerish. All rights reserved.
//

import UIKit
import SwiftSocket
import UIKit.UIGestureRecognizerSubclass
import Flow

let cellSize: CGFloat = 17 // In case you have a smaller phone - they won't behave automatically (because I went full lazy on this one..)

struct ConnectionInfo {
    let address: String
    let port: Int32
}

extension ConnectionInfo {
    init?(connectionString: String) {
        let components = connectionString.split(separator: ":")
        guard components.count == 2 else { return nil }
        guard let port = Int32(components[1]) else { return nil }
        self.init(address: String(components[0]), port: port)
    }
    
    var connectionString: String {
        return "\(address):\(port)"
    }
}

var cachedConnection: ConnectionInfo? {
    get {
        return UserDefaults.standard.string(forKey: "cached-connection").flatMap { ConnectionInfo(connectionString: $0) }
    }
    set {
        if let info = newValue {
            UserDefaults.standard.set(info.connectionString, forKey: "cached-connection")
        } else {
            UserDefaults.standard.removeObject(forKey: "cached-connection")
        }
        UserDefaults.standard.synchronize()
    }
}

class CellView: UIView {
    var state: Bool = true {
        didSet {
            if oldValue != state {
                self.backgroundColor = state ? .yellow : UIColor(white: 1, alpha: 0.2)
                guard let client = client else { return }
                let int = tag + (state ? 1000 : 0)
                _ = client.send(string: "\(int),")
            }
        }
    }
}

var client: TCPClient?

class ViewController: UIViewController {
    
    @IBOutlet weak var clearButton: UIButton!
    @IBOutlet weak var ipTextField: UITextField!
    var stack: UIStackView!
    var lastCell: CellView?
    var adding = true
    
    let bag = DisposeBag()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        stack = UIStackView()
        stack.axis = .vertical
        stack.spacing = 1
        stack.translatesAutoresizingMaskIntoConstraints = false
        stack.distribution = .fillEqually
        var index = 0
        
        for _ in 1...8 {
            let rowStack = UIStackView()
            stack.addArrangedSubview(rowStack)
            rowStack.axis = .horizontal
            rowStack.distribution = .fillEqually
            rowStack.spacing = 1
            rowStack.translatesAutoresizingMaskIntoConstraints = false
            for _ in 1...(8*5) {
                let view = CellView()
                view.state = false
                view.translatesAutoresizingMaskIntoConstraints = false
                view.heightAnchor.constraint(equalToConstant: cellSize).isActive = true
                view.widthAnchor.constraint(equalTo: view.heightAnchor).isActive = true
                view.tag = index
                rowStack.addArrangedSubview(view)
                index += 1
            }
        }
        view.addSubview(stack)
        stack.centerYAnchor.constraint(equalTo: view.centerYAnchor).isActive = true
        stack.centerXAnchor.constraint(equalTo: view.centerXAnchor).isActive = true
        
        let panGesture = UIPanGestureRecognizer(target: self, action: #selector(ViewController.handlePan(gesture:)))
        panGesture.maximumNumberOfTouches = 1
        panGesture.minimumNumberOfTouches = 1
        panGesture.delegate = self
        stack.addGestureRecognizer(panGesture)
        
        let tapGesture = SingleTouchDownGestureRecognizer(target: self, action: #selector(ViewController.handleTap(gesture:)))
        tapGesture.delegate = self
        stack.addGestureRecognizer(tapGesture)
        
        
        // Setup view bindings and connect
        
        ipTextField.text = cachedConnection?.connectionString ?? ""
        bag += clearButton.onValue(sendClearCommand)
        bag += ipTextField.atOnce().compactMap { ConnectionInfo(connectionString: $0) }.onValue {
            cachedConnection = $0
            client?.close()
            self.connect()
        }
        bag += ipTextField.signal(for: .editingDidEndOnExit).onValue {
            self.ipTextField.resignFirstResponder()
        }
        
        
    }
    
    @objc func handlePan(gesture: UIPanGestureRecognizer) {
        let loc = gesture.location(in: gesture.view)
        guard let cell = cellAt(point: loc), cell != lastCell else { return }
        lastCell = cell
        
        switch gesture.state {
        case .ended, .cancelled, .failed:
            lastCell = nil
        default:
            break
        }
        
        cell.state = adding
    }
    
    @objc func handleTap(gesture: UIGestureRecognizer) {
        let loc = gesture.location(in: gesture.view)
        guard let cell = cellAt(point: loc), cell != lastCell else { return }
        adding = !cell.state
        cell.state = adding
    }
    
    func connect() {
        guard let connectionInfo = cachedConnection else { return }
        client = TCPClient(connectionInfo: connectionInfo)
        
        switch client!.connect(timeout: 10) {
        case .success:
            print("Connected")
            sendClearCommand()
        case .failure(_):
            // TODO: We should really add some sort of auto connect here...
            print("💩")
        }
    }
    
    func cellAt(point: CGPoint) -> CellView? {
        for rowStack in stack.arrangedSubviews as! [UIStackView] {
            for cell in rowStack.arrangedSubviews as! [CellView] {
                let frame = rowStack.convert(cell.frame, to: stack)
                if frame.contains(point) {
                    return cell
                }
            }
        }
        return nil
    }
    
    func sendClearCommand() {
        guard let client = client else { return }
        
        for rowStack in stack.arrangedSubviews as! [UIStackView] {
            for cell in rowStack.arrangedSubviews as! [CellView] {
                cell.state = false
            }
        }
        
        _ = client.send(string: "clear,")
    }
}

extension ViewController: UIGestureRecognizerDelegate {
    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldRecognizeSimultaneouslyWith otherGestureRecognizer: UIGestureRecognizer) -> Bool {
        return true
    }
}

extension TCPClient {
    convenience init(connectionInfo: ConnectionInfo) {
        self.init(address: connectionInfo.address, port: connectionInfo.port)
    }
}

// https://stackoverflow.com/questions/15628133/uitapgesturerecognizer-make-it-work-on-touch-down-not-touch-up/15629234
class SingleTouchDownGestureRecognizer: UIGestureRecognizer {
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent) {
        if self.state == .possible {
            self.state = .recognized
        }
    }
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent) {
        self.state = .failed
    }
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent) {
        self.state = .failed
    }
}
