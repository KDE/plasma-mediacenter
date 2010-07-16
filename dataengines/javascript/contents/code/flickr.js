/***************************************************************************
 *   Copyright 2010 by Onur-Hayri Bakici <thehayro@gmail.com               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

//var result = new Array();
var flickrObject = null;
//addon
function flickrmain(webmediaengine)
{
  print("Hello flickr!");
  flickrObject = new Flickr(webmediaengine);
  return flickrObject;
}


function Flickr(webmediaengine)
{
  this.baseUrl = "http://www.flickr.com/services/rest/";
  this.apiKey = "api_key=df52d83b7fe03f1de079da2c300f3203";
  this.mediaengine = webmediaengine;
}

Flickr.prototype.searchMedia = function(queryParams)
{
  
  var query = queryParams['text'];
  print(query);
 
  var url = this.baseUrl + "?method=flickr.photos.search&" + this.apiKey + "&text=" + query;
  print(url);

  //doRequest(this.mediaengine, this.url, DataCallBack, FinishedCallBack);
  var io = this.mediaengine.getUrl(url);
  var array = new Array();
  var result = "";
  io.data.connect(function (job, data)
  {
    result += data.valueOf();
  });
  
  io.finished.connect(function(job)
  {
    print("Job done");
    print("Parsing...");
    var objDom = new XMLDoc(result, xmlError);
    var objDomTree = objDom.docNode;
    
    var photoNodes = objDomTree.getElements("photos")[0].getElements("photo");
    if (photoNodes.length == 0)
      print("photonodes is empty");
    print(photoNodes.length);
    
    for (var i = 0; i < photoNodes.length; i++)
    {
      
      var webmedia = new WebMedia();
      var farm = photoNodes[i].getAttribute("farm");
      var server = photoNodes[i].getAttribute("server");
      var secret = photoNodes[i].getAttribute("secret");
      webmedia.type = "photo";
      webmedia.id = photoNodes[i].getAttribute("id");
      
      webmedia.title = photoNodes[i].getAttribute("title");
      webmedia.description = "";
  //    webmedia.keywords ="photo";
      webmedia.author = photoNodes[i].getAttribute("owner");
      webmedia.updated = "";
      webmedia.published = "";
      webmedia.link = "http://farm" + farm + ".static.flickr.com/" + server + "/" + webmedia.id + "_" + secret + "_m.jpg";
      webmedia.height = "";
      webmedia.width = "";
      webmedia.size = "1";
      webmedia.collection = "";
      webmedia.thumbnailLink = "http://farm" + farm + ".static.flickr.com/" + server + "/" + webmedia.id + "_" + secret + "_t.jpg";
      //MediaDataCollection.push(webmedia);
      getPhotoInfo(webmedia);
    }
    print("Parsing done.");
    });
}



Flickr.prototype.toString = function()
{
  //workaround for now, until the plugin search function is implemented
  return "Flickr";
}


//gets the more information about the photo from a webmedia element
function getPhotoInfo(webmedia)
{
  print("getting more info");
  var url = flickrObject.baseUrl + "?method=flickr.photos.getInfo&" + flickrObject.apiKey + "&photo_id=" + webmedia.id;
  var io = flickrObject.mediaengine.getUrl(url);
  print(url);
  var result = "";
  
  io.data.connect(function(job, data)
  {
    result += data.valueOf();
  });
  
  io.finished.connect(function(job)
  {
    //parsing
    var objDom = new XMLDoc(result, xmlError);
    var objDomTree = objDom.docNode;
    var infoNodes = objDomTree.getElements("photo")[0];
    
    webmedia.description = infoNodes.getElements("description")[0].getText();
    
    var tags = infoNodes.getElements("tags")[0].getElements("tag");
    print(tags.length);
    for (var i = 0; i < tags.length; i++)
    {
      webmedia.keywords.push(tags[i].getText());
    }
    var dateNode = infoNodes.getElements("dates")[0];
    webmedia.updated = UnixToDate(dateNode.getAttribute("lastupdate"));
    webmedia.published = UnixToDate(dateNode.getAttribute("posted"));
    setData(webmedia.id, webmedia.toArray());
  });
}



function xmlError(e)
{
  print(e);
}


//helper function
function UnixToDate(unixTimeStamp)
{
  return new Date(unixTimeStamp * 1000);
}
