import QtQuick
import QtQuick.Controls
import QtGraphicalEffects

Button {
    id: control
    
    property bool highlighted: false
    
    contentItem: Text {
        text: control.text
        font: control.font
        color: control.highlighted ? 
            (control.pressed ? "#ffffff" : "#ffffff") : 
            (control.pressed ? "#000000" : "#000000")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    
    background: Rectangle {
        implicitWidth: 80
        implicitHeight: 32
        radius: 4
        
        color: {
            if (control.highlighted) {
                return control.pressed ? "#0063b1" : 
                       control.hovered ? "#0078d4" : "#0067c0"
            } else {
                return control.pressed ? "#e0e0e0" : 
                       control.hovered ? "#f0f0f0" : "#ffffff"
            }
        }
        
        border.color: control.highlighted ? "transparent" : "#e0e0e0"
        border.width: 1
        
        // 阴影效果
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: control.pressed ? 1 : 2
            radius: control.pressed ? 4 : 8
            samples: 17
            color: "#20000000"
        }
    }
    
    // 鼠标悬停效果
    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }
    
    // 点击动画
    states: State {
        name: "pressed"
        when: control.pressed
        PropertyChanges {
            target: control
            scale: 0.98
        }
    }
    
    transitions: Transition {
        from: ""
        to: "pressed"
        reversible: true
        NumberAnimation {
            properties: "scale"
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }
} 