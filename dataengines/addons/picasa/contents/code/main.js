/***************************************************************************
 *   Copyright 2010, 2011 by Onur-Hayri Bakici <thehayro@gmail.com>        *
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
  this.name = "picasa";
}

Picasa.prototype.searchMedia = function(queryParams)
{
  var text = queryParams['text'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  var media = queryParams['media'];
  
  if (media != "" || media)
    outputWarningMessage(this.name, media, WarningTypes[PARAMIGNORED]);
  
  // calculate the index of the first picture in each page
  var startIndex = (maxResults * page) - (maxResults - 1);;
  
  var url = buildUrl(this.baseUrl + "all",
		     {
		      "q" : text,
		      "max-results" : maxResults,
		      "start-index" : startIndex
		     }
	    );
  var result = "";
  
  doRequest(engine, url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      try 
	      {
		print("Job done");
		print("Parsing...");
		//parse xml here
		var mediaNS = "http://search.yahoo.com/mrss/";
		var gphotoNS = "http://schemas.google.com/photos/2007";
		
		var parser = new DOMImplementation();
		var domDoc = parser.loadXML(result);
		if (!domDoc)
		{
		  outputErrorMessage(this.name, "", result);
		  return;
		}
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		if (mediaNodes.length == 0)
		{
		  outputErrorMessage(this.name, "", text, ErrorTypes[NORESULTS]);
		  return;
		}
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  var webmedia = new WebMedia();
		  var mediaNodesItem = mediaNodes.item(i);
		  
		  //potential nullable items
		  var summaryItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		  var keywordsItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0);
		  
		  webmedia.type = MediaTypes[PHOTO];
		  webmedia.id = getID();
		  webmedia.providerSpecificID = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "id").item(0).getFirstChild().getNodeValue();
		  webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
		  if (summaryItem)
		    webmedia.description = summaryItem.getNodeValue();
		  if (keywordsItem)
		    webmedia.keywords = keywordsItem.getFirstChild().getNodeValue().split(",");
		  webmedia.user = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagNameNS(gphotoNS, "user").item(0).getFirstChild().getNodeValue();
		  webmedia.authodNick = webmedia.user = mediaNodesItem.getElementsByTagName("author").item(0).getElementsByTagNameNS(gphotoNS, "nickname").item(0).getFirstChild().getNodeValue();
		  webmedia.updated = new Date();
		  webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		  webmedia.published = new Date();
		  webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		  webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
		  webmedia.width = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "width").item(0).getFirstChild().getNodeValue();
		  webmedia.height = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "height").item(0).getFirstChild().getNodeValue();
		  for (var j = 0; j < mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").length; j++)
		  {
		    webmedia.addThumbnail(mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").item(j).getAttributes().getNamedItem("url").getNodeValue(),
					  mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").item(j).getAttributes().getNamedItem("height").getNodeValue(),
					  mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").item(j).getAttributes().getNamedItem("height").getNodeValue());
		  }
		  webmedia.collectionID = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "albumid").item(0).getFirstChild().getNodeValue();
		  setData(webmedia.id, webmedia.toArray());
		}
	      }catch(e)
	      {
		if (typeof e.message === "undefined")
		  outputErrorMessage(this.name, "", result);
		else
		  outputErrorMessage(this.name, "", e.message);
	      }
	    }
  );
}

Picasa.prototype.searchCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "searchCollection", ErrorTypes[FUNCTIONNOTPROVIDED]);
}


Picasa.prototype.getUploadedUserMedia = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  // calculate the index of the first picture in each page
  var startIndex = (maxResults * page) - (maxResults - 1);;
  
  var url = buildUrl(this.baseUrl + "user/" + user,
		    {
		      "kind" : "photo",
		      "start-index" : startIndex,
		      "max-results" : maxResults
		    }
	    );
  var result = "";
  print(url);
  doRequest(engine, url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      try
	      {
		print("Parsing...");
		
		//parse xml here
		
		// important namespaces
		var mediaNS = "http://search.yahoo.com/mrss/";
		var gphotoNS = "http://schemas.google.com/photos/2007";
		
		var parser = new DOMImplementation();
		var domDoc = parser.loadXML(result);
		
		var docRoot = domDoc.getDocumentElement();
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		if (mediaNodes.length == 0)
		{
		  outputErrorMessage(this.name, "", user, ErrorTypes[NORESULTS]);
		  return;
		}
		var thumb = null;
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  var webmedia = new WebMedia();
		  var mediaNodesItem = mediaNodes.item(i);
		  
		  //potential nullable items
		  var summaryItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		  var keywordsItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0);    
		  
		  webmedia.type = MediaTypes[PHOTO];
		  webmedia.id = getID();//mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		  webmedia.providerSpecificID = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "id").item(0).getFirstChild().getNodeValue();
		  // those two elements are in each webmedia entry
		  webmedia.authorNick = docRoot.getElementsByTagNameNS(gphotoNS, "nickname").item(0).getFirstChild().getNodeValue();
		  webmedia.user = docRoot.getElementsByTagNameNS(gphotoNS, "user").item(0).getFirstChild().getNodeValue();
		  webmedia.provider = this.name;
		  webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
		  if (summaryItem)
		    webmedia.description = summaryItem.getNodeValue();
		  
		  if (keywordsItem )
		    webmedia.keywords = keywordsItem.getFirstChild().getNodeValue().split(",");
		  
		  webmedia.updated = new Date();
		  webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		  webmedia.published = new Date();
		  webmedia.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		  webmedia.link = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0).getAttributes().getNamedItem("url").getNodeValue();
		  webmedia.width = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "width").item(0).getFirstChild().getNodeValue();
		  webmedia.height = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "height").item(0).getFirstChild().getNodeValue();
		  
		  for (var j = 0; j < mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").length; j++)
		  {
		    webmedia.addThumbnail(mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").item(j).getAttributes().getNamedItem("url").getNodeValue(),
					  mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").item(j).getAttributes().getNamedItem("height").getNodeValue(),
					  mediaNodesItem.getElementsByTagNameNS(mediaNS, "group").item(0).getElementsByTagNameNS(mediaNS, "thumbnail").item(j).getAttributes().getNamedItem("height").getNodeValue());
		  }
		  webmedia.collectionID = mediaNodesItem.getElementsByTagNameNS(gphotoNS, "albumid").item(0).getFirstChild().getNodeValue();
		  setData(webmedia.id, webmedia.toArray());
		}
	      }catch(e)
	      {
		if (typeof e.message === "undefined")
		  outputErrorMessage(this.name, "", result);     
		else
		  outputErrorMessage(this.name, "", e.message);
	      }
	  }
  );
}

Picasa.prototype.getUploadedUserCollection = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  // the index of the first shown entry
  var startIndex = (maxResults * page) - (maxResults - 1);;
  
  var url = buildUrl(this.baseUrl + "user/" + user,
		     {
		      "kind" : "album",
		      "start-index" : startIndex,
		      "max-results" : maxResults
		     }
	    );
  var result = "";
  doRequest(engine, url,
	    function(job, data)
	    {
	      result += data.valueOf();
	    },
	    function(job)
	    {
	      try
	      {
		var gphotoNS = "http://schemas.google.com/photos/2007";
		//parse xml here
		var parser = new DOMImplementation();
		
		var domDoc = parser.loadXML(result);
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("entry");
		if (mediaNodes.length == 0)
		{
		  outputErrorMessage(this.name, "", user, ErrorTypes[NORESULTS]);
		  return;
		}
		
		for (var i = 0; i < mediaNodes.length; i++)
		{
		  var webmediaCollection = new WebMediaCollection();
		  var mediaNodesItem = mediaNodes.item(i);
		  var mediaEntriesUrl = null;
		  
		  //potential nullable item
		  var descriptionItem = mediaNodesItem.getElementsByTagName("summary").item(0).getFirstChild();
		  
		  webmediaCollection.id = getID();
		  webmediaCollection.providerSpecificID = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		  webmediaCollection.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();     
		  if (descriptionItem)
		    webmediaCollection.description = descriptionItem.getNodeValue();
		  //webmedia.keywords = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild().getNodeValue().split(",");
		  webmediaCollection.link =  mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		  webmediaCollection.user = user;
		  webmediaCollection.updated = new Date();
		  webmediaCollection.updated.setISO8601(mediaNodesItem.getElementsByTagName("updated").item(0).getFirstChild().getNodeValue());
		  webmediaCollection.published = new Date();
		  webmediaCollection.updated.setISO8601(mediaNodesItem.getElementsByTagName("published").item(0).getFirstChild().getNodeValue());
		  setData(webmediaCollection.id, webmediaCollection.toArray());
		}
	      }catch(e)
	      {
		if (typeof e.message === "undefined")
		  outputErrorMessage(this.name, "", result);     
		else
		  outputErrorMessage(this.name, "", e.message);
	      }
	    }
  ); 
}

Picasa.prototype.toString = function()
{
  return "Picasa";
}

registerAddon(Picasa)