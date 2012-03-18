/***************************************************************************
 *   Copyright 2010, 2011 by Onur-Hayri Bakici <thehayro@gmail.com         *
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

var blipObject = null;

function Blip()
{
  print("Hello blip");
  this.baseUrl = "http://www.blip.tv/";
  this.name = "blip";
  blipObject = this;
}

Blip.prototype.searchMedia = function(queryParams)
{
  var text = queryParams['text'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  if (maxResults != "")
    outputWarningMessage(this.name, "max-results", WarningTypes[PARAMIGNORED]);
  if (page != "")
    outputWarningMessage(this.name, "page", WarningTypes[PARAMIGNORED]);
 
  var url = buildUrl(this.baseUrl + "search/view/",
		     {
		       "search" : text,
		       "skin": "api"
		     }
		    );
  print(url);

  var result = "";
  
  doRequest(engine, url,
    function (job, data)
    {
      result += data.valueOf();
    },
    function(job)
    {
      try{  
	var objDom = new XMLDoc(result, xmlError);
	var objDomTree = objDom.docNode;
	
	//blip tv has an inconsisties return of errors
	if (objDomTree.getElements("notice")[0] != null)
	{
	  outputErrorMessage(blipObject.name, "", objDomTree.getElements("notice")[0].getText(), ErrorTypes[INTERNALERROR]);
	  return;
	}
	if (blipObject.isErrorMessage(objDomTree))
	  return;
	var photoNodes = objDomTree.getElements("payload")[0].getElements("asset");
	if (photoNodes.length == 0)
	{
	  outputErrorMessage(blipObject.name, "", text, ErrorTypes[NORESULTS]);
	  return;
	}
	for (var i = 0; i < photoNodes.length; i++)
	{
	
	  //get only the first entry of the media list
	  var mediaItem = photoNodes[i].getElements("mediaList")[0].getElements("media");
	  var webmedia = new WebMedia();
	  webmedia.type = MediaTypes[VIDEO];
	  
	  webmedia.id = getID();
	  webmedia.providerSpecificID = photoNodes[i].getElements("id")[0].getText();
	  webmedia.provider = blipObject.name;
	  webmedia.title = photoNodes[i].getElements("title")[0].getText();
	  webmedia.description = photoNodes[i].getElements("description")[0].getText();
	  
	  var tagItems = photoNodes[i].getElements("tags")[0].getElements("string");
	  for (var j = 0; j < tagItems.length; j++)
	  {
	    webmedia.keywords[j] = tagItems[j].getText();
	  }
	  
	  var webRoleItem = null;
	  // get the "web"-role from the video
	  for (var j = 0; j < mediaItem; j++)
	  {
	    if (mediaItem[j].getElements("role")[0].getText() == "Web")
	      webRoleItem = mediaItem[j];
	  }
	  // "web"-role not found. Taking the first one
	  webRoleItem = mediaItem[0];
	  webmedia.user = photoNodes[i].getElements("createdBy")[0].getElements("login")[0].getText();
	  webmedia.published = new Date(parseInt(photoNodes[i].getElements("timestamp")[0].getText()));
	  webmedia.updated = new Date(parseInt(photoNodes[i].getElements("modified")[0].getText()));
	  webmedia.link = webRoleItem.getElements("link")[0].getAttribute("href");
	  webmedia.height = webRoleItem.getElements("height")[0].getText();
	  webmedia.width = webRoleItem.getElements("width")[0].getText();
	  webmedia.size = webRoleItem.getElements("size")[0].getText();
    //     webmedia.thumbnailLink = "http://farm" + farm + ".static.flickr.com/" + server + "/" + webmedia.id + "_" + secret + "_t.jpg";
	  //MediaDataCollection.push(webmedia);
	  setData(webmedia.id, webmedia.toArray());
	}
      }
      catch(e)
      {
	outputErrorMessage(blipObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
      }
    }
  );
}


Blip.prototype.seachCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "searchCollection", ErrorTypes[FUNCTIONNOTPROVIDED]);
}

Blip.prototype.getUploadedUserMedia = function(queryParams)
{
  outputErrorMessage(this.name, "", "getUploadedUserMedia", ErrorTypes[FUNCTIONNOTPROVIDED]);
}

Blip.prototype.getUploadedUserCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "getUploadedUserCollection", ErrorTypes[FUNCTIONNOTPROVIDED]);
}

Blip.prototype.toString = function()
{
  return "Blip";
}

Blip.prototype.isErrorMessage = function(objDomTree)
{
  if (objDomTree.getElements("status")[0].getText() == "ERROR")
  {
    var msg = objDomTree.getElements("error")[0].getElements("message")[0].getText();
    setData(this.ToString(), "Error", msg);
    return true;
  }
  return false;
}


function xmlError(e)
{
  print("Hello Error: " + e);
}


registerAddon(Blip)