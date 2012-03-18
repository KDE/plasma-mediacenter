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

var ytObject = null;

function Youtube()
{
  print("Hello youtube");
  this.baseUrl = "http://gdata.youtube.com/feeds/api/";
  this.name = "youtube";
  ytObject = this;
}

Youtube.prototype.searchMedia = function(queryParams)
{
  var text= queryParams['text'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  // calculate the index of the first picture in each page
  var startIndex = (maxResults * page) - (maxResults - 1);;
  
  var url = buildUrl(this.baseUrl + "videos", 
		     {
		       "q" : text,
		       "max-results" : maxResults,
		       "start-index" : startIndex
		     });
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
		  outputErrorMessage(ytObject.name, "", text, ErrorTypes[NORESULTS]);
		  return;
		}
		
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  var webmedia = new WebMedia();
		  var mediaNodesItem = mediaNodes.item(i);
		  if (mediaNodes.length == 0)
		  {
		    outputErrorMessage(ytObject.name, "", text, ErrorTypes[NORESULTS]);
		    return;
		  }
		  //potential nullable items
		  var keywordsItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild();
		  var contentItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0);
		  
		  webmedia.type = MediaTypes[VIDEO];
		  webmedia.id = getID();
		  webmedia.providerSpecificID = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		  webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
		  webmedia.description = mediaNodesItem.getElementsByTagNameNS(mediaNS, "description").item(0).getFirstChild().getNodeValue();
		  
		  if (keywordsItem)
		    webmedia.keywords = keywordsItem.getNodeValue().split(",");
		  
		  webmedia.user = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagName("name").item(0).getFirstChild().getNodeValue();
		  webmedia.updated = new Date();
		  webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		  webmedia.published = new Date();
		  webmedia.published.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		  
		  // get the default content item
		  if (contentItem){
		    for (var c = 0; c < mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").length; c++)
		    {
		      var cItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(c);
		      if (cItem.getAttributes().getNamedItem("isDefault").getNodeValue() == 'true'){
			webmedia.link = cItem.getAttributes().getNamedItem("url").getNodeValue();
			webmedia.videoFormat = cItem.getAttributes().getNamedItem("type").getNodeValue();
			webmedia.videoDuration = cItem.getAttributes().getNamedItem("duration").getNodeValue();
			break;
		      }
		    }
		  }
		  for (var t = 0; t < mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").length; t++)
		  {
		    var tItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(t);
		    webmedia.addThumbnail(tItem.getAttributes().getNamedItem("url").getNodeValue(), tItem.getAttributes().getNamedItem("height"),
					  tItem.getAttributes().getNamedItem("width"));
		  }
		  setData(webmedia.id, webmedia.toArray());
		  webmedia.toArrayString();
		}
	      }catch(e)
	      {
		outputErrorMessage(ytObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
	      }
	    }
  );
}


Youtube.prototype.searchCollection = function(queryParams)
{
  var text = queryParams['text'];
  print(query);
  var url = buildUrl(this.baseUrl + "playlists/snippets",
		     {
		       "q": text,
		       "v" : "2"
		     });
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
		  outputErrorMessage(ytObject.name, "", text, ErrorTypes[NORESULTS]);
		  return;
		}
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  
		  var webmediaCollection = new WebMediaCollection();
		  var mediaNodesItem = mediaNodes.item(i);
		  var mediaEntriesUrl = null;
		  var idForEngine = mediaNodesItem.getElementsByTagNameNS(ytNS, "playlistId").item(0).getFirstChild().getNodeValue();
		  
		  //potential nullable items
		  var descriptionItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		  
		  webmediaCollection.id = getID();
		  webmediaCollection.providerSpecificID = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		  webmediaCollection.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();     
		  if (descriptionItem)
		    webmediaCollection.description = descriptionItem.getNodeValue();
		  //webmedia.keywords = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild().getNodeValue().split(",");
		  webmediaCollection.link =  mediaNodesItem.getElementsByTagName("content").item(0).getAttributes().getNamedItem("src").getNodeValue();
		  webmediaCollection.author = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagName("name").item(0).getFirstChild().getNodeValue();
		  webmediaCollection.updated = new Date();
		  //webmediaCollection.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		  webmediaCollection.published = new Date();
		  webmediaCollection.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		  //webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
		  //webmedia.width = 0;
		  //webmedia.height = 0;
		  //webmedia.thumbnailLink = mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(0).getAttributes().getNamedItem("url").getNodeValue();
		  setData(webmediaCollection.id, webmediaCollection.toArray());
		}
	      }catch(e)
	      {
		outputErrorMessage(ytObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
	      }
	    }
  );
}

Youtube.prototype.getUploadedUserMedia = function(queryParams)
{
  
}

Youtube.prototype.getUploadedUserCollection = function(queryParams)
{
  
}

Youtube.prototype.toString = function()
{
  return "Youtube";
}

registerAddon(Youtube)