import QtQuick 2.2
import QtQuick.Extras 1.4

Rectangle {

    id: indicator
    width: 100
    height: 100
    color: "#cccccc"

    StatusIndicator {
        anchors.centerIn: parent
        color: "green"
    }

    Connections {
        target: indicator;
        onMystringChanged: {
            // 这里的value是signal信号函数里面的参数
            color: "red"
            console.log("value:" + value )

        }
    }
}
