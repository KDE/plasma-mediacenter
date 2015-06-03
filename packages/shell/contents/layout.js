for (var i = 0; i < screenCount; ++i) {
    var id = createActivity("Desktop", "org.kde.mediacentercontainment");
    var desktopsArray = desktopsForActivity(id);
    print(desktopsArray.length);
    for( var j = 0; j < desktopsArray.length; j++) {
        desktopsArray[j].wallpaperPlugin = 'org.kde.image';
        desktopsArray[j].addWidget("org.kde.plasma.analogclock");
        desktopsArray[j].addWidget("org.kde.plasma.digitalclock");
        desktopsArray[j].addWidget("org.kde.plasma.comic");
    }
}
