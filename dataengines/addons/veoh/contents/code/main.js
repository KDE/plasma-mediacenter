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
  veohObject = this;
}

Veoh.prototype.searchMedia = function(queryParams)
{
  var query = queryParams['text'];
  if (query === "" || typeof query === "undefined")
  {
    print("no query defined");
    return;
  }
  var url = this.baseUrl + "?method=veoh.search.video&apiKey=" + this.apiKey + "&userQuery=" + query;
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
	  print("photonodes is empty");
	
	for (var i = 0; i < videoNodes.length; i++)
	{
	  try
	  {
	    var webmedia = new WebMedia();
	    
	    webmedia.type = MediaType.video;
	    
	    webmedia.id = videoNodes[i].getAttribute("videoId");
	    webmedia.title = videoNodes[i].getAttribute("title");
	    webmedia.description = videoNodes[i].getAttribute("description");
	    webmedia.keywords = videoNodes[i].getAttribute("tags").split(" ");
	    webmedia.author = videoNodes[i].getAttribute("username");
	    webmedia.published = new Date(videoNodes[i].getAttribute("dateAdded"));
	    webmedia.link = videoNodes[i].getAttribute("previewUrl");
	    webmedia.thumbnailLink = videoNodes[i].getAttribute("highResImage");
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

Veoh.prototype.searchCollection = function(queryParams)
{
  var query = queryParams['text'];
  if (query === "" || typeof(query) == "undefined")
    return;
  var url = this.baseUrl + "?method=veoh.search.collection&userQuery=" + query + "&apiKey=" + this.apiKey;
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
	if (typeof(videosetNodes) === "undefined" || videosetNodes == null || videosetNodes.length == 0)
	{
	  print("No collections");
	  return;
	}
	for (var i = 0; i < videosetNodes.length; ++i)
	{
	  try
	  {
	    var webmediaCollection = new WebMediaCollection();
	    webmediaCollection.id = videosetNodes[i].getAttribute("collectionId");
	    webmediaCollection.title = videosetNodes[i].getAttribute("title");
	    webmediaCollection.description = videosetNodes[i].getAttribute("description");
	    webmediaCollection.updated = videosetNodes[i].getAttribute("dateLastModified");
	    webmediaCollection.author = videosetNodes[i].getAttribute("username");
	    setData(webmediaCollection.id, webmediaCollection.toArray());
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

Veoh.prototype.toString = function()
{
  return "Veoh";
}

Veoh.prototype.isErrorMessage = function(objDomTree)
{
  if (objDomTree.getAttribute("stat") == "fail")
  {
    var msg = objDomTree.getElements("errorList")[0].getAttribute("errorMessage");
    setData(this.toString(), "Error", msg);
    return true;
  }
  return false;
}

function xmlError(e)
{
  print(e);
}

registerAddon(Veoh);