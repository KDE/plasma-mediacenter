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

var veohObject = null;

function Veoh()
{
  print("Hello veoh");
  this.baseUrl = "http://www.veoh.com/rest/v2/execute.xml";
  this.apiKey = "A73F9B59-0A24-23C3-B376-1A57CCA2C00B";
  this.name = "veoh";
  veohObject = this;
}

Veoh.prototype.searchMedia = function(queryParams)
{
  var text = queryParams['text'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  // zero based index
  var offset = (maxResults  * page) - maxResults;
  var url = buildUrl(this.baseUrl, 
		    {
		      "method" : "veoh.search.video",
		      "apiKey" : this.apiKey,
		      "userQuery" : text,
		      "maxResults" : maxResults,
		      "offset" : offset
		    });
  print(url);
  var result = "";
  doRequest(engine, url, 
    function (job, data)
    {
      result += data.valueOf();
    },
    function(job)
    {
      try
      {
	var objDom = new XMLDoc(result, xmlError);
	var objDomTree = objDom.docNode;
	
	if (veohObject.isErrorMessage(objDomTree))
	  return;
	
	var videoNodes = objDomTree.getElements("videoList")[0].getElements("video");
	if (videoNodes.length == 0)
	{
	  errorMessage(veohObject.name, "", text, ErrorTypes[NORESULTS]);
	  return;
	}
	for (var i = 0; i < videoNodes.length; i++)
	{
	  var webmedia = new WebMedia();
	  
	  webmedia.type = MediaTypes[VIDEO];
	  
	  webmedia.id = getID();
	  webmedia.providerSpecificID = videoNodes[i].getAttribute("videoId");
	  webmedia.title = videoNodes[i].getAttribute("title");
	  webmedia.description = videoNodes[i].getAttribute("description");
	  webmedia.keywords = videoNodes[i].getAttribute("tags").split(" ");
	  webmedia.user = videoNodes[i].getAttribute("username");
	  webmedia.published = new Date(videoNodes[i].getAttribute("dateAdded"));
	  webmedia.link = videoNodes[i].getAttribute("previewUrl");
	  webmedia.addThumbnai(videoNodes[i].getAttribute("highResImage"), 0, 0);
	  setData(webmedia.id, webmedia.toArray());
	}
      }catch(e)
      {
	outputErrorMessage(veohObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
      }
    }
  );
}

Veoh.prototype.searchCollection = function(queryParams)
{
  var text = queryParams['text'];
  
  var url = buildUrl(this.baseUrl, 
		     {
		       "method" : "veoh.search.collection",
		       "userQuery" : text,
		       "apiKey" : this.apiKey
		     });
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
	var objDom = new XMLDoc(result, xmlError);
	var objDomTree = objDom.docNode;
	
	var videosetNodes = objDomTree.getElements("collections")[0].getElements("collection");
	if (!videosetNodes || videosetNodes.length == 0)
	{
	  outputErrorMessage(veohObject.name, "", text, ErrorTypes[NORESULTS]);
	  return;
	}
	for (var i = 0; i < videosetNodes.length; ++i)
	{
	  var webmediaCollection = new WebMediaCollection();
	  webmediaCollection.id = getID();
	  webmediaCollection.providerSpecificID = videosetNodes[i].getAttribute("collectionId");
	  webmediaCollection.title = videosetNodes[i].getAttribute("title");
	  webmediaCollection.description = videosetNodes[i].getAttribute("description");
	  webmediaCollection.updated = videosetNodes[i].getAttribute("dateLastModified");
	  webmediaCollection.user = videosetNodes[i].getAttribute("username");
	  webmediaCollection.thumbnail = videosetNodes[i].getAttribute("highResImageUrl");
	  setData(webmediaCollection.id, webmediaCollection.toArray());
	}
      }catch(e)
      {
	outputErrorMessage(veohObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
      }
    }
  );
}

Veoh.prototype.getUploadedUserMedia = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  // zero based index
  var offset = (maxResults * page) - maxResults;
  var url = buildUrl(this.baseUrl,
		     {
		       "method" : "veoh.people.getPublishedVideos",
		       "username" : user,
		       "maxResults" : maxResults,
		       "offset" : offset,
		       "apiKey" : this.apiKey
		     }
		  );
  print(url);
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
		var objDom = new XMLDoc(result, xmlError);
		var objDomTree = objDom.docNode;
		
		if (veohObject.isErrorMessage(objDomTree))
		  return;
		
		var videoNodes = objDomTree.getElements("videoList")[0].getElements("video");
		if (videoNodes.length == 0)
		{
		  errorMessage(veohObject.name, "", text, ErrorTypes[NORESULTS]);
		  return;
		}
		for (var i = 0; i < videoNodes.length; i++)
		{
		  var webmedia = new WebMedia();
		  
		  webmedia.type = MediaTypes[VIDEO];
		  
		  webmedia.id = getID();
		  webmedia.providerSpecificID = videoNodes[i].getAttribute("videoId");
		  webmedia.title = videoNodes[i].getAttribute("title");
		  webmedia.description = videoNodes[i].getAttribute("description");
		  webmedia.keywords = videoNodes[i].getAttribute("tags").split(" ");
		  webmedia.user = videoNodes[i].getAttribute("username");
		  webmedia.published = new Date(videoNodes[i].getAttribute("dateAdded"));
		  webmedia.link = videoNodes[i].getAttribute("previewUrl");
		  webmedia.addThumbnail(videoNodes[i].getAttribute("highResImage"), 0, 0);
		  setData(webmedia.id, webmedia.toArray());
		}
	      }catch(e)
	      {
		outputErrorMessage(veohObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
	      }
	    }
	  );
}

Veoh.prototype.getUploadedUserCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "getUploadedUserCollection", ErrorTypes[FUNCTIONNOTPROVIDED]);
}

Veoh.prototype.toString = function()
{
  return "Veoh";
}

Veoh.prototype.isErrorMessage = function(objDomTree)
{
  if (objDomTree.getAttribute("stat") == "fail")
  {
    var msg = objDomTree.getElements("errorList")[0].getAttribute("errorMessage");
    outputErrorMessage(this.name,"", "Error", msg);
    return true;
  }
  return false;
}

function xmlError(e)
{
  print(e);
}

registerAddon(Veoh);