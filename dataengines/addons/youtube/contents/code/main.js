/***************************************************************************
 *   Copyright 2010 by Onur-Hayri Bakici <thehayro@gmail.com>              *
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

function Youtube()
{
  print("Hello youtube");
  this.baseUrl = "http://gdata.youtube.com/feeds/api/";
}

Youtube.prototype.searchMedia = function(queryParams)
{
  var query = queryParams['text'];
  if (query === "" || typeof query === "undefined")
  {
    print("no query defined");
    return;
  }
  print(query);
  var url = this.baseUrl + "videos?q=" + query;
  print(url);
  var result = "";
  
  doRequest(engine, url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      try{
		print("Job done");
		print("Parsing...");
		//parse xml here
		var mediaNS = "http://search.yahoo.com/mrss/";
		var ytNS = "http://gdata.youtube.com/schemas/2007";
		var parser = new DOMImplementation();
		
		var domDoc = parser.loadXML(result);
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		
		if (mediaNodes.length == 0)
		{
		  setData("Youtube", "Error", "No results for " + query + " found");
		  return;
		}
		
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  try{
		    var webmedia = new WebMedia();
		    var mediaNodesItem = mediaNodes.item(i);
		    if (mediaNodes.length == 0)
		    {
		      setData("Youtube", "Error", "No results for " + query + " found");
		      return;
		    }
		    //potential nullable items
		    var keywordsItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild();
		    var contentItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0);
		    
		    webmedia.type = MediaType.video;
		    webmedia.id = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		    webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
		    webmedia.description = mediaNodesItem.getElementsByTagNameNS(mediaNS, "description").item(0).getFirstChild().getNodeValue();
		    
		    if (typeof(keywordsItem) !== "undefined" && keywordsItem != null)
		      webmedia.keywords = keywordsItem.getNodeValue().split(",");
		    
		    webmedia.author = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagName("name").item(0).getFirstChild().getNodeValue();
		    webmedia.updated = new Date();
		    webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		    webmedia.published = new Date();
		    webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		    
		    if (typeof(contentItem) !== "undefined" && contentItem != null)
		      webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
		    
		    webmedia.width = 0;
		    webmedia.height = 0;
		    webmedia.thumbnailLink = mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(0).getAttributes().getNamedItem("url").getNodeValue();
		    setData(webmedia.id, webmedia.toArray());
		  }catch(e)
		  {
		    print("A problem occured while parsing.");
		  }
		}
	      }catch(e)
	      {
		print("A problem occured while parsing.");
	      }
	    }
  );
}


Youtube.prototype.searchCollection = function(queryParams)
{
  var query = queryParams['text'];
  print(query);
  var url = this.baseUrl + "playlists/snippets?q=" + query + "&v=2";
  var ytNS = "http://gdata.youtube.com/schemas/2007";
  var result = "";
  
  doRequest(engine,url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      try{
		//parse xml here
		var parser = new DOMImplementation();
		
		var domDoc = parser.loadXML(result);
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		if (mediaNodes.length == 0)
		{
		  setData("Youtube", "Error", "No results for " + query + " found");
		  return;
		}
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  try{
		    var webmediaC = new WebMediaCollection();
		    var mediaNodesItem = mediaNodes.item(i);
		    var mediaEntriesUrl = null;
		    var idForEngine = mediaNodesItem.getElementsByTagNameNS(ytNS, "playlistId").item(0).getFirstChild().getNodeValue();
		    
		    //potential nullable items
		    var descriptionItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		    
		    webmediaC.id = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		    webmediaC.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();     
		    if (typeof(descriptionItem) !== "undefined" && descriptionItem != null)
		      webmediaC.description = descriptionItem.getNodeValue();
		    //webmedia.keywords = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild().getNodeValue().split(",");
		    webmediaC.link =  mediaNodesItem.getElementsByTagName("content").item(0).getAttributes().getNamedItem("src").getNodeValue();
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
		  }catch(e)
		  {
		    print("A problem occured while parsing:" + e.message);
		  }
		}
	      }catch(e)
	      {
		print("A problem occured while parsing:" + e.message);
	      }
	    }
  );
}



Youtube.prototype.toString = function()
{
  return "Youtube";
}

registerAddon(Youtube)