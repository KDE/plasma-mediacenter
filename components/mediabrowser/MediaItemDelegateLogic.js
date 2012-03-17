function checkAndLoad(loader)
{
    if (typeof(decoration) == "string") {
        if (decoration.search('[a-z]+://') == 0) {
            loadImage(loader);
        } else {
            loadIcon(loader);
        }
    } else if (typeof(decoration) == "object") {
        loadIcon(loader);
    }
}

function loadImage(loader)
{
    rootColumn.source = decoration;
    loader.sourceComponent = delegateItemImageComponent;
}

function loadIcon(loader)
{
    rootColumn.source = decoration;
    loader.sourceComponent = delegateItemIconComponent;
}
