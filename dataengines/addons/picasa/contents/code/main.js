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

function Picasa()
{
  print("Hello picasa");
  this.baseUrl = "http://picasaweb.google.com/data/feed/api/";
  this.mediaengine = GetWebMediaEngine();
}

Picasa.prototype.searchMedia = function(queryParams)
{
  var query = queryParams['text'];
  if (query === "" || typeof query === "undefined")
  {
    print("no query defined");
    return;
  }
  print(query);
  var url = this.baseUrl + "all?q=" + query + "&max-results=25";
  print(url);
  var result = "";
  
  doRequest(engine, url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      try {
		print("Job done");
		print("Parsing...");
		//parse xml here
		var mediaNS = "http://search.yahoo.com/mrss/";
		var gphotoNS = "http://schemas.google.com/photos/2007";
		
		var parser = new DOMImplementation();
		var domDoc = parser.loadXML(result);
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		if (mediaNodes.length == 0)
		{
		  setData("Picasa", "Error", "No results for " + query + " found");
		  return;
		}
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  try{
		    var webmedia = new WebMedia();
		    var mediaNodesItem = mediaNodes.item(i);
		    
		    //potential nullable items
		    var summaryItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		    var keywordsItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0);
		    
		    
		    webmedia.type = MediaType.photo;
		    webmedia.id = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		    
		    webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
		    if (typeof(summaryItem) !== "undefined" && summaryItem != null)
		      webmedia.description = summaryItem.getNodeValue();
		    
		    if (typeof(keywordsItem) !== "undefined" && keywordsItem != null)
		      webmedia.keywords = keywordsItem.getFirstChild().getNodeValue().split(",");
		    
		    webmedia.author = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagNameNS(gphotoNS, "user").item(0).getFirstChild().getNodeValue();
		    webmedia.updated = new Date();
		    webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		    webmedia.published = new Date();
		    webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		    webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
		    webmedia.width = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "width").item(0).getFirstChild().getNodeValue();
		    webmedia.height = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "height").item(0).getFirstChild().getNodeValue();
		    webmedia.thumbnailLink = mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(0).getAttributes().getNamedItem("url").getNodeValue();
		    webmedia.collectionID = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "albumid").item(0).getFirstChild().getNodeValue();
		    setData(webmedia.id, webmedia.toArray());
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

Picasa.prototype.searchCollection = function(queryParams)
{
  var user = queryParams['user'];
  if (user === "" || typeof(user) == "undefined")
    return;
  
  var url = this.baseUrl + "user/" + user;
  var result = "";
  doRequest(engine, url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      if (result == "Unknown user.")
	      {
		print("Unknown user " + user);
		return;
	      }
	      try{
		var gphotoNS = "http://schemas.google.com/photos/2007";
		//parse xml here
		var parser = new DOMImplementation();
		
		var domDoc = parser.loadXML(result);
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  try{
		    var webmediaC = new WebMediaCollection();
		    var mediaNodesItem = mediaNodes.item(i);
		    var mediaEntriesUrl = null;
		    var idForEngine = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "id").item(0).getFirstChild().getNodeValue();
		    
		    //potential nullable items
		    var descriptionItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		    
		    webmediaC.id = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		    webmediaC.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();     
		    if (typeof(descriptionItem) !== "undefined" && descriptionItem != null)
		      webmediaC.description = descriptionItem.getNodeValue();
		    //webmedia.keywords = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild().getNodeValue().split(",");
		    webmediaC.link =  mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		    webmediaC.author = user;
		    webmediaC.updated = new Date();
		    webmediaC.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		    webmediaC.published = new Date();
		    webmediaC.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		    setData(idForEngine, webmediaC.toArray());
		  }catch(e)
		  {
		    print("A problem occured while parsing: " + e.message);
		  }
		}
	      }catch(e)
	      {
		print("A problem occured while parsing: " + e.message);
	      }
	    }
  );    
}

Picasa.prototype.toString = function()
{
  return "Picasa";
}

registerAddon(Picasa)