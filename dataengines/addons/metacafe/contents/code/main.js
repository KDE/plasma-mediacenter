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
var mcObject = null;

function Metacafe()
{
  print("Hello metacafe");
  this.baseUrl = "http://www.metacafe.com/api/videos/";
  this.name = "metacafe";
  mcObject = this;
}

Metacafe.prototype.searchMedia = function(queryParams)
{
  var text = queryParams['text'];
  var maxResults = queryParams['max-results'];;
  var page = queryParams['page'];
  // zero based index
  var startIndex = (maxResults * page) - maxResults;
  
  var url = buildUrl(this.baseUrl,
		     {
		      "q" : text,
		      "max-results" : maxResults,
		      "start-index" : startIndex
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
	      try{
		//parse xml here
		var mediaNS = "http://search.yahoo.com/mrss/";
		var parser = new DOMImplementation();
		
		var domDoc = parser.loadXML(result);
		var docRoot = domDoc.getDocumentElement();
		
		var mediaNodes =  docRoot.getElementsByTagName("item");
		
		if (mediaNodes.length == 0)
		{
		  outputErrorMessage(mcObject.name, "", text, ErrorTypes[NORESULTS]);
		  return;
		}
		for (var i = 0; i < mediaNodes.length; ++i)
		{
		  var webmedia = new WebMedia();
		  var mediaNodesItem = mediaNodes.item(i);
		  var contentItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "content").item(0);
		  
		  //potential nullable item
		  var keywordsItem = mediaNodesItem.getElementsByTagNameNS(mediaNS, "keywords").item(0).getFirstChild();
		  
		  webmedia.type = MediaTypes[VIDEO];
		  webmedia.id = getID();
		  webmedia.providerSpecificID = mediaNodesItem.getElementsByTagName("id").item(0).getFirstChild().getNodeValue();
		  webmedia.title = mediaNodesItem.getElementsByTagName("title").item(0).getFirstChild().getNodeValue();
		  webmedia.description = mediaNodesItem.getElementsByTagNameNS(mediaNS, "description").item(0).getFirstChild().getNodeValue();
		  
		  if (keywordsItem)
		    webmedia.keywords = keywordsItem.getNodeValue().split(",");
		  
		  webmedia.user = mediaNodesItem.getElementsByTagName("author").item(0).getFirstChild().getNodeValue();
		  webmedia.published = mediaNodesItem.getElementsByTagName("pubDate").item(0).getFirstChild().getNodeValue();
		  webmedia.link = contentItem.getAttributes().getNamedItem("url").getNodeValue();
		  webmedia.width = contentItem.getAttributes().getNamedItem("height").getNodeValue();
		  webmedia.height = contentItem.getAttributes().getNamedItem("width").getNodeValue();
		  webmedia.videoDuration = contentItem.getAttributes().getNamedItem("duration").getNodeValue();
		  webmedia.videoFormat = contentItem.getAttributes().getNamedItem("type").getNodeValue();
		  webmedia.addThumbnail(mediaNodesItem.getElementsByTagNameNS(mediaNS, "thumbnail").item(0).getAttributes().getNamedItem("url").getNodeValue(), 0, 0);
		  setData(webmedia.id, webmedia.toArray());
		}
	      }catch(e)
	      {
		outputErrorMessage(mcObject.name, "", e.message, ErorTypes[PARSINGERROR]);
	      }
	    }
  );
}


Metacafe.prototype.searchCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "searchCollection", ErorTypes[FUNCTIONNOTPROVIDED]);
}

Metacafe.prototype.getUploadedUserMedia = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
}

Metacafe.prototype.getUploadedUserCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "getUploadedUserCollection", ErorTypes[FUNCTIONNOTPROVIDED]);
}

Metacafe.prototype.toString = function()
{
  return "Metacafe";
}

function xmlError(e)
{
  print(e);
}

registerAddon(Metacafe)