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


// Configuration
let tcpAddress = "10.0.1.16"
let port: Int32 = 5204

let cellSize: CGFloat = 17 // In case you have a smaller phone - they won't behave automatically (because I went full lazy on this one..)
// =====

class CellView: UIView {
    var state: Bool = true {
        didSet {
            if oldValue != state {
                self.backgroundColor = state ? .yellow : .black
                guard let client = client else { return }
                let int = tag + (state ? 1000 : 0)
                _ = client.send(string: "\(int),")
            }
        }
    }
}

var client: TCPClient?

class ViewController: UIViewController {
    
    var stack: UIStackView!
    var lastCell: CellView?
    var adding = true
    
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
        
        client = TCPClient(address: tcpAddress, port: port)
        
        switch client!.connect(timeout: 10) {
        case .success:
            print("Connected")
            sendClearCommand()
        case .failure(_):
            // TODO: We should really add some sort of auto connect here...
            print("💩")
        }
        
        let panGesture = UIPanGestureRecognizer(target: self, action: #selector(ViewController.handlePan(gesture:)))
        panGesture.maximumNumberOfTouches = 1
        panGesture.minimumNumberOfTouches = 1
        panGesture.delegate = self
        stack.addGestureRecognizer(panGesture)
        
        let tapGesture = SingleTouchDownGestureRecognizer(target: self, action: #selector(ViewController.handleTap(gesture:)))
        tapGesture.delegate = self
        stack.addGestureRecognizer(tapGesture)
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
    
    @IBAction func clearButtonTapped(_ sender: Any) {
        sendClearCommand()
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
