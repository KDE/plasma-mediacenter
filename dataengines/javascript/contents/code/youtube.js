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

var youtubeObject = null;

function youtubemain(webmediaengine)
{
  print("Hello youtube!");
  youtubeObject = new Youtube(webmediaengine);
  return youtubeObject;
}


function Youtube(webmediaengine)
{
  this.mediaengine = webmediaengine;
  this.baseUrl = "http://gdata.youtube.com/feeds/api/";
}

Youtube.prototype.searchMedia = function(queryParams)
{
  var query = queryParams['text'];
  print(query);
  var url = this.baseUrl + "videos?q=" + query;
  print(url);
  var io = this.mediaengine.getUrl(url);
  var result = "";
  
  io.data.connect(function(job, data)
  {
    result += data.valueOf();
  });
  
  io.finished.connect(function(job)
  {
    print("Job done");
    print("Parsing...");
    //parse xml here
    var mediaNS = "http://search.yahoo.com/mrss/";
    var ytNS = "http://gdata.youtube.com/schemas/2007";
    var parser = new DOMImplementation();
    
    var domDoc = parser.loadXML(result);
    var docRoot = domDoc.getDocumentElement();
    
    var mediaNodes =  docRoot.getElementsByTagName("entry");
    
    for (var i = 0; i < mediaNodes.length; i++)
    {
      var webmedia = new WebMedia();
      var mediaNodesItem = mediaNodes.item(i);
      
      webmedia.type = "video";
      webmedia.id = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
      webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
      webmedia.description = mediaNodesItem.getElementsByTagNameNS(mediaNS, "description").item(0).getFirstChild().getNodeValue();
      webmedia.keywords = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild().getNodeValue().split(",");
      
      webmedia.author = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagName("name").item(0).getFirstChild().getNodeValue();
      webmedia.updated = new Date();
      webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
      webmedia.published = new Date();
      webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
      webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
      webmedia.width = 0;
      webmedia.height = 0;
      webmedia.thumbnailLink = mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(0).getAttributes().getNamedItem("url").getNodeValue();
      setData(webmedia.id, webmedia.toArray());
    }
  });
}


Youtube.prototype.searchCollection = function(queryParams)
{
  var query = queryParams['text'];
  print(query);
  var url = this.baseUrl + "playlists/snippets?q=" + query + "&v=2";
  var ytNS = "http://gdata.youtube.com/schemas/2007";
  //print(url);
  var io = this.mediaengine.getUrl(url);
  var result = "";
  
  io.data.connect(function(job, data)
  {
    result += data.valueOf();
  });
  
  io.finished.connect(function(job)
  {
    //parse xml here
    var parser = new DOMImplementation();
    
    var domDoc = parser.loadXML(result);
    var docRoot = domDoc.getDocumentElement();
    
    var mediaNodes =  docRoot.getElementsByTagName("entry");
    
    for (var i = 0; i < mediaNodes.length; i++)
    {
      var webmediaC = new WebMediaCollection();
      var mediaNodesItem = mediaNodes.item(i);
      var mediaEntriesUrl = null;
      var idForEngine = mediaNodesItem.getElementsByTagNameNS(ytNS, "playlistId").item(0).getFirstChild().getNodeValue();
      webmediaC.id = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
      webmediaC.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();     
      webmediaC.description = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild().getNodeValue();
      //webmedia.keywords = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild().getNodeValue().split(",");
      mediaEntriesUrl =  mediaNodesItem.getElementsByTagName("content").item(0).getAttributes().getNamedItem("src").getNodeValue();
      webmediaC.author = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagName("name").item(0).getFirstChild().getNodeValue();
      webmediaC.updated = new Date();
      webmediaC.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
      webmediaC.published = new Date();
      webmediaC.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
      //webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
      //webmedia.width = 0;
      //webmedia.height = 0;
      //webmedia.thumbnailLink = mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(0).getAttributes().getNamedItem("url").getNodeValue();
      setData(idForEngine, webmediaC.toArray());
      
    }
  });
}



Youtube.prototype.toString = function()
{
  //workaround for now, until the plugin search function is implemented
  return "Youtube";
}
