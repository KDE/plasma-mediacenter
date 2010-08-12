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

var blipObject = null;

function Blip()
{
  print("Hello blip");
  this.baseUrl = "http://www.blip.tv/";
  blipObject = this;
}

Blip.prototype.searchMedia = function(queryParams)
{
  var query = queryParams['text'];
  if (query === "" || typeof query === "undefined")
  {
    print("no query defined");
    return;
  }
  print(query);
 
  var url = this.baseUrl + "search/view/?search=" + query + "&skin=api";
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
	
	//blip tv has an inconsisten return of errors
	if (objDomTree.getElements("notice")[0] != null)
	{
	  setData(blipObject.toString(), "Error", objDomTree.getElements("notice")[0].getText());
	  return;
	}
	/*if (blipObject.isErrorMessage(objDomTree))
	  return;*/
	var photoNodes = objDomTree.getElements("payload")[0].getElements("asset");
	if (photoNodes.length == 0)
	  print("photonodes is empty");	
	for (var i = 0; i < photoNodes.length; i++)
	{
	  try{
	    //get only the first entry of the media list
	    var mediaItem = photoNodes[i].getElements("mediaList")[0].getElements("media")[0];
	    var webmedia = new WebMedia();
	    webmedia.type = MediaType.video;
	    
	    webmedia.id = photoNodes[i].getElements("id")[0].getText();
	    
	    webmedia.title = photoNodes[i].getElements("title")[0].getText();
	    webmedia.description = photoNodes[i].getElements("description")[0].getText();
	    
	    var tagItems = photoNodes[i].getElements("tags")[0].getElements("string");
	    for (var j = 0; j < tagItems.length; ++j)
	    {
	      webmedia.keywords[j] = tagItems[j].getText();
	    }
	    
	    webmedia.author = photoNodes[i].getElements("createdBy")[0].getElements("login")[0].getText();
	    webmedia.published = new Date(photoNodes[i].getElements("modified")[0].getText());
	    webmedia.link = mediaItem.getElements("link")[0].getAttribute("href");
	    webmedia.height = mediaItem.getElements("height")[0].getText();
	    webmedia.width = mediaItem.getElements("width")[0].getText();
	    webmedia.size = mediaItem.getElements("size")[0].getText();;
      //     webmedia.thumbnailLink = "http://farm" + farm + ".static.flickr.com/" + server + "/" + webmedia.id + "_" + secret + "_t.jpg";
	    //MediaDataCollection.push(webmedia);
	    setData(webmedia.id, webmedia.toArray());
	  }
	  catch(e)
	  {
	    print("A problem occured while parsing:" + e.message);
	  }
	}
      }
      catch(e)
      {
	print("A problem occured while parsing:" + e.message);
      }
    }
  );
}


Blip.prototype.seachCollection = function(queryParams)
{
  
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