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

var flickrObject = null;

function Flickr()
{
  print("Hello flickr");
  this.baseUrl = "http://www.flickr.com/services/rest/";
  this.apiKey = "df52d83b7fe03f1de079da2c300f3203";
  this.name = "flickr";
  flickrObject = this;
}

Flickr.prototype.searchMedia = function(queryParams)
{
  var text = queryParams['text'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  var media = queryParams['media'];

  if (media == "" || !media)
    media = "photos";
  
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
  
  var url = buildUrl(this.baseUrl, 
		      {
			"method" : "flickr.photos.search",
		        "api_key" : this.apiKey,
		        "text" : text,
			"media" : media,
		        "extras" : "description,date_upload,owner_name,last_update,tags,machine_tags,o_dims,media,url_t,url_o",
			"per_page" : max-results,
			"page" : page
		      }
		    );
  //print(url2);
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
	// we cannot access the this object. we have to access it indirectly
	if (flickrObject.isErrorMessage(objDomTree))
	  return;
	var photoNodes = objDomTree.getElements("photos")[0].getElements("photo");
	if (photoNodes.length == 0)
	{
	  outputErrorMessage(this.name, "", text, ErrorTypes[NORESULTS]);
	  return;
	}
	for (var i = 0; i < photoNodes.length; i++)
	{
	  try
	  {
	    var webmedia = new WebMedia();
	    if (photoNodes[i].getAttribute("media") == "photo")
	      webmedia.type = MediaTypes[PHOTO];
	    else
	      webmedia.type = MediaTypes[VIDEO];
	    
	    webmedia.id = getID();
	    webmedia.providerSpecificID = photoNodes[i].getAttribute("id");
	    webmedia.title = photoNodes[i].getAttribute("title");
	    webmedia.description = photoNodes[i].getElements("description")[0].getText();
	    
	    webmedia.keywords = photoNodes[i].getAttribute("tags").split(" ");
	    
	    webmedia.user = photoNodes[i].getAttribute("ownername");
	    webmedia.updated = photoNodes[i].getAttribute("last_update");;
	    webmedia.published = photoNodes[i].getAttribute("dateupload");
	    webmedia.link = photoNodes[i].getAttribute("url_o");
	    webmedia.height = photoNodes[i].getAttribute("height_o");
	    webmedia.width = photoNodes[i].getAttribute("width_o");
	    webmedia.size = "";
	  //  webmedia.collection = "";
	    webmedia.addThumbnail(photoNodes[i].getAttribute("url_t"),
				  photoNodes[i].getAttribute("height_t"),
				  photoNodes[i].getAttribute("width_t"));
	    setData(webmedia.id, webmedia.toArray());
	  }catch(e)
	  {
	    print("A problem occured while parsing: " + e.message);
	    if (typeof e.message === "undefined")
	      outputErrorMessage(this.name, "", result);     
	    else
	      outputErrorMessage(this.name, "", e.message);
	  }
	}
	print("Parsing done");
      }catch(e)
      {
	print("A problem occured while parsing: " + e.message);
	if (typeof e.message === "undefined")
	  outputErrorMessage(this.name, "", result);     
	else
	  outputErrorMessage(this.name, "", e.message);
      }
    }
  );
}

Flickr.prototype.searchCollection = function(queryParams)
{
  outputErrorMessage(this.name, "", "searchCollection", ErrorTypes[FUNCTIONNOTPROVIDED]);
}


Flickr.prototype.getUploadedUserMedia = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  var collectionID = queryParams['providerCollectionID'];
  
  if (collectionID)
    outputWarningMessage(this.name, collectionID, WarningTypes[PARAMIGNORED]);
  
  var result = "";
  this.getNSID(user, 
		function(nsid)
		{
		  if (!nsid)
		  {
		    outputErrorMessage(flickrObject.name, "", "Fetching username " + user + " failed");
		    return;
		  }
		  
		  var url = buildUrl(flickrObject.baseUrl,
				{
				  "method" : "flickr.people.getPhotosOf",
				  "api_key" : flickrObject.apiKey,
				  "user_id" : nsid,
				  "extras" : "description,date_upload,owner_name,last_update,tags,machine_tags,o_dims,media,url_t,url_o",
				  "per_page" : maxResults,
				  "page" : page
				}
			    );
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
				  // we cannot access the this object. we have to access it indirectly
				  if (flickrObject.isErrorMessage(objDomTree))
				    return;
				  var photoNodes = objDomTree.getElements("photos")[0].getElements("photo");
				  if (photoNodes.length == 0)
				  {
				    outputErrorMessage(this.name, "", text, ErrorTypes[NORESULTS]);
				    return;
				  }
				  for (var i = 0; i < photoNodes.length; i++)
				  {
				    try
				    {
				      var webmedia = new WebMedia();
				      if (photoNodes[i].getAttribute("media") == "photo")
					webmedia.type = MediaTypes[PHOTO];
				      else
					webmedia.type = MediaTypes[VIDEO];
				      
				      webmedia.id = getID();
				      webmedia.providerSpecificID = photoNodes[i].getAttribute("id");
				      webmedia.title = photoNodes[i].getAttribute("title");
				      webmedia.description = photoNodes[i].getElements("description")[0].getText();
				      
				      webmedia.keywords = photoNodes[i].getAttribute("tags").split(" ");
				      
				      webmedia.user = photoNodes[i].getAttribute("ownername");
				      webmedia.updated = photoNodes[i].getAttribute("last_update");;
				      webmedia.published = photoNodes[i].getAttribute("dateupload");
				      webmedia.link = photoNodes[i].getAttribute("url_o");
				      webmedia.height = photoNodes[i].getAttribute("height_o");
				      webmedia.width = photoNodes[i].getAttribute("width_o");
				      webmedia.size = "";
				      webmedia.addThumbnail(photoNodes[i].getAttribute("url_t"),
							    photoNodes[i].getAttribute("height_t"),
							    photoNodes[i].getAttribute("width_t"));
				      setData(webmedia.id, webmedia.toArray());
				    }catch(e)
				    {
				      print("A problem occured while parsing: " + e.message);
				      if (typeof e.message === "undefined")
					outputErrorMessage(fObj.name, "", result);     
				      else
					outputErrorMessage(fObj.name, "", e.message);
				    }
				  }
				  print("Parsing done");
				}catch(e)
				{
				  print("A problem occured while parsing: " + e.message);
				  if (typeof e.message === "undefined")
				    outputErrorMessage(fObj.name, "", result);     
				  else
				    outputErrorMessage(fObj.name, "", e.message);
				}
			      }
			  );
		}
	      );
  
}

Flickr.prototype.getUploadedUserCollection = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  
  var result = "";
  // getting the internal flickr user id
  this.getNSID(user,
	       function(nsid)
	       {
		  var url = buildUrl(flickrObject.baseUrl, 
				     {
					"method" : "flickr.photosets.getList",
					"api_key" : flickrObject.apiKey,
					"user_id" : nsid
				     }
			    );
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
				  
				  if (flickrObject.isErrorMessage(objDomTree))
				    return;
				  var photosetNodes = objDomTree.getElements("photosets")[0].getElements("photoset");
				  if (!photosetNodes || photosetNodes.length == 0)
				  {
				    outputErrorMessage(flickrObject.name, "", text, ErrorTypes[NORESULTS]);
				    return;
				  }
				  for (var i = 0; i < photosetNodes.length; ++i)
				  {
				    try
				    {
				      var webmediaCollection = new WebMediaCollection();
				      var farm = photosetNodes[i].getAttribute("farm");
				      var server = photosetNodes[i].getAttribute("server");
				      webmediaCollection.id = getID();
				      webmediaCollection.providerSpecificID = photosetNodes[i].getAttribute("id");
				      webmediaCollection.provider = flickrObject.name;
				      webmediaCollection.title = photosetNodes[i].getElements("title")[0].getText();
				      webmediaCollection.description = photosetNodes[i].getElements("description")[0].getText();
				      setData(webmediaCollection.id, webmediaCollection.toArray());
				    }
				    catch(e)
				    {
				      print("A problem occured while parsing: " + e.message);
				      if (typeof e.message === "undefined")
					outputErrorMessage(flickrObject.name, "", result);     
				      else
					outputErrorMessage(flickrObject.name, "", e.message);
				    }
				  }
				}catch(e)
				{
				  print("A problem occured while parsing: " + e.message);
				  if (typeof e.message === "undefined")
				    outputErrorMessage(flickrObject.name, "", result);     
				  else
				    outputErrorMessage(flickrObject.name, "", e.message);
				}
			      }
			    );
		  }
		);
}

Flickr.prototype.toString = function()
{
  return "Flickr";
}


Flickr.prototype.getNSID = function(username, onNSIDRetrievedCallback)
{
  var url = buildUrl(this.baseUrl,
		    {
		      "method" : "flickr.people.findByUsername",
		      "api_key" : this.apiKey,
		      "username" : username
		    }
	    );
  var nsid = null;
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
		if (flickrObject.isErrorMessage(objDomTree))
		  return;
		nsid = objDomTree.getElements("user")[0].getAttribute("nsid");
		onNSIDRetrievedCallback(nsid);
	      }catch(e)
	      {
		print("A problem occured while parsing: " + e.message);
		if (typeof e.message === "undefined")
		  outputErrorMessage(this.name, "", result);     
		else
		  outputErrorMessage(this.name, "", e.message);
	      }
	    }
	  );
}

Flickr.prototype.isErrorMessage = function(objDomTree)
{
  if (objDomTree.getAttribute("stat") == "fail")
  {
    var msg = objDomTree.getElements("err")[0].getAttribute("msg");
    var code = objDomTree.getElements("err")[0].getAttribute("code");
    outputErrorMessage(this.name, code, "Server response", msg);
    return true;
  }
  return false;
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