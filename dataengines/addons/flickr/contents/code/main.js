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

var flickrObject = null;

function Flickr()
{
  print("Hello flickr");
  this.baseUrl = "http://www.flickr.com/services/rest/";
  this.apiKey = "df52d83b7fe03f1de079da2c300f3203";
  flickrObject = this;
  this.mediaengine = GetWebMediaEngine();
}

Flickr.prototype.searchMedia = function(queryParams)
{
  
  var query = queryParams['text'];
  if (query === "" || typeof query === "undefined")
  {
    print("no query defined");
    return;
  }
  
  /*
  we can search for videos as well, with the extras=media parameter
  description: the description of this medium
  date_upload: the upload date_upload
  last_update: the update date
  media: the mediatype (video or photo)
  owner_name: the name of the author
  o_dims: original dimensions
  tags: the tags of this medium
  url_t: the thumbnailLink
  url_o: the url of the original media link
  
  */
  var url = this.baseUrl + "?method=flickr.photos.search&api_key=" + 
  this.apiKey + "&text=" + query + 
  "&extras=description,date_upload,owner_name,last_update,"+
  "tags,machine_tags,o_dims,media,url_t,url_o";
  print(url);

  var array = new Array();
  var result = "";
  doRequest(engine, url, 
    function (job, data)
    {
      result += data.valueOf();
    },
    function(job)
    {
      try{
	print("Job done");
	print("Parsing...");
	var objDom = new XMLDoc(result, xmlError);
	var objDomTree = objDom.docNode;
	
	var photoNodes = objDomTree.getElements("photos")[0].getElements("photo");
	if (photoNodes.length == 0)
	  print("photonodes is empty");
	
	print(photoNodes.length);
	
	for (var i = 0; i < photoNodes.length; i++)
	{
	  try{
	    var webmedia = new WebMedia();
	    
	    if (photoNodes[i].getAttribute("media") == "photo")
	      webmedia.type = MediaType.photo;
	    else
	      webmedia.type = MediaType.video;
	    
	    webmedia.id = photoNodes[i].getAttribute("id");
	    webmedia.title = photoNodes[i].getAttribute("title");
	    webmedia.description = photoNodes[i].getElements("description")[0].getText();
	    
	    webmedia.keywords = photoNodes[i].getAttribute("tags").split(" ");
	    
	    webmedia.author = photoNodes[i].getAttribute("ownername");
	    webmedia.updated = photoNodes[i].getAttribute("last_update");;
	    webmedia.published = photoNodes[i].getAttribute("dateupload");
	    webmedia.link = photoNodes[i].getAttribute("url_o");
	    webmedia.height = photoNodes[i].getAttribute("height_o");
	    webmedia.width = photoNodes[i].getAttribute("width_o");
	    webmedia.size = "1";
	    webmedia.collection = "";
	    webmedia.thumbnailLink = photoNodes[i].getAttribute("url_t");
	    setData(webmedia.id, webmedia.toArray());
	  }
	  catch(e)
	  {
	    print("A problem occured while parsing:" + e.message);
	  }
	}
	print("Parsing done.");
      }catch(e)
      {
	print("A problem occured while parsing:" + e.message);
      }
    }
  );
}

Flickr.prototype.searchCollection = function(queryParams)
{
  var user = queryParams['user'];
  if (user === "" || typeof(user) == "undefined")
    return;
  
  // getting the internal flickr user id
  var url = this.baseUrl + "?method=flickr.people.findByUsername&api_key=" + this.apiKey + "&username=" + user;
  print(url);
  
  var result = "";
  var nsid = "";
  doRequest(engine, url,
    function(job, data)
    {
      result += data.valueOf();
    },
    function(job)
    {
      try{
	print("Job finished");
	var objDom = new XMLDoc(result, xmlError);
	var objDomTree = objDom.docNode;
	
	nsid = objDomTree.getElements("user")[0].getAttribute("nsid");
	print(nsid);
	
	if (nsid == null && typeof(nsid) === "undefined")
	{
	  print("empty");
	  return;
	}
	
	//now get the list of albums from the user
	url = flickrObject.baseUrl + "?method=flickr.photosets.getList&api_key=" + flickrObject.apiKey + "&user_id=" + nsid;
	result = "";
	print(url);
	doRequest(engine, url,
	  function(job, data)
	  {
	    result += data.valueOf();
	  },
	  function(job)
	  {
	    try{
	      var objDom = new XMLDoc(result, xmlError);
	      var objDomTree = objDom.docNode;
	      
	      var photosetNodes = objDomTree.getElements("photosets")[0].getElements("photoset");
	      if (typeof(photosetNodes) === "undefined" || photosetNodes == null || photosetNodes.length == 0)
	      {
		print("No collections");
		return;
	      }
	      for (var i = 0; i < photosetNodes.length; ++i)
	      {
		try
		{
		  var webmediaCollection = new WebMediaCollection();
		  var farm = photosetNodes[i].getAttribute("farm");
		  var server = photosetNodes[i].getAttribute("server");
		  webmediaCollection.id = photosetNodes[i].getAttribute("id");
		  webmediaCollection.title = photosetNodes[i].getElements("title")[0].getText();
		  webmediaCollection.description = photosetNodes[i].getElements("description")[0].getText();
		  webmediaCollection.author = user;
		  setData(webmediaCollection.id, webmediaCollection.toArray());
		}
		catch(e)
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
      }catch(e)
      {
	print("A problem occured while parsing: " + e.message);
      }
    }
  );
}

Flickr.prototype.toString = function()
{
  //workaround for now, until the plugin search function is implemented
  return "Flickr";
}



function xmlError(e)
{
  print(e);
}


//helper function
function UnixToDate(unixTimeStamp)
{
  return new Date(unixTimeStamp * 1000);
}

registerAddon(Flickr)

