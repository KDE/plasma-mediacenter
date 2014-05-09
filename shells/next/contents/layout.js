
var panel = new Panel
panel.screen = 0
panel.location = 'top'
panel.addWidget("org.kde.plasma.kickoff")
panel.addWidget("org.kde.plasma.pager")
panel.addWidget("org.kde.plasma.taskmanager")
panel.addWidget("org.kde.plasma.systemtray")
panel.addWidget("org.kde.plasma.digitalclock")

for (var i = 0; i < screenCount; ++i) {
    var desktop = new Activity
    desktop.name = i18n("Pmc")
    desktop.screen = i
    desktop.wallpaperPlugin = 'org.kde.image'
    print("IN PLASMA MEDIACENTER SHELLLLL ******************************************************");
    //var clock = desktop.addWidget("org.kde.analogclock");
}
