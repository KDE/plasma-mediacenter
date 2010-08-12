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

var photobucketObject = null;

function Photobucket()
{
  print("Hello photobucket");
  this.consumerKey = "149830244";
  this.consumerSecret = "0549aaac2e9552d174f73835c2d1edf3";
  this.baseUrl = "http://api.photobucket.com";
  photobucketObject = this;
}

Photobucket.prototype.searchMedia = function(queryParams)
{
  var query = queryParams['text'];
  if (query === "" || query === "undefined")
  {
    print("No query defined");
    return;
  }

  var url = this.buildUrl(this.baseUrl + "/search/" + query + "/image", "format=xml");
  print(url);
  
  if (url == "" || url == undefined)
  {
    print("No photobucket url found");
    return;
  }
  
  var result = "";
  
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

		if (photobucketObject.isErrorMessage(objDomTree))
		  return;
		
		var photoNodes = objDomTree.getElements("content")[0].getElements("result")[0].getElements("primary")[0].getElements("media");
		if (photoNodes.length == 0)
		{
		  setData("Photobucket", "Error", "No results for " + query + " found");
		  return;
		}
		print(photoNodes.length);

		for (var i = 0; i < photoNodes.length; i++)
		{
		  try{
		    var webmedia = new WebMedia();
		    
		    //potential nullable items
		    var descriptionItem = photoNodes[i].getElements("decription")[0];
		    
		    webmedia.type = MediaType.photo;
		    
		    //in photobucket the attribute 'description_id' is somehow always an empty string
		    webmedia.id = i;//photoNodes[i].getAttribute("description_id");

		    webmedia.title = photoNodes[i].getElements("title")[0].getText();
		    
		    if (typeof(descriptionItem) !== "undefined" && descriptionItem != null)
		      webmedia.description = descriptionItem.getText();
		    
		    webmedia.author = photoNodes[i].getAttribute("username");
		    webmedia.published = photoNodes[i].getAttribute("uploaddate");
		    webmedia.link = photoNodes[i].getElements("url")[0].getText();
		    webmedia.thumbnailLink = photoNodes[i].getElements("thumb")[0].getText();
		    setData(webmedia.id, webmedia.toArray());
	      //       getPhotoInfo2(webmedia);
		  }
		  catch(e)
		  {
		    print("A problem occured while parsing: " + e.message);
		  }
		}
	      }
	      catch(e)
	      {
		print("A problem occured while parsing: " + e.message);
	      }
	    }
  );  
}


Photobucket.prototype.searchCollection = function(queryParams)
{
  var user = queryParams['user'];
  if (user === "" || user === "undefined")
  {
    print("no user defined");
    return;
  }
  var url = this.buildUrl(this.baseUrl + "/album/" + user, "format=xml");
  print(url);
  if (url == "" || url == undefined)//&view=flat&recurse=1
  {
    print("No photobucket url found");
    return;
  }
  
  var result = "";
  
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
		
		if (photobucketObject.isErrorMessage(objDomTree))
		  return;
		var albumNodes = objDomTree.getElements("content")[0].getElements("album");
		if (albumNodes.length == 0)
		  return;
		for (var i = 0; i < albumNodes.length; i++)
		{
		  try{
		    var webmediaColl = new WebMediaCollection();
		    
		    //potential nullable items
		    var descriptionItem = albumNodes[i].getElements("decription")[0];
		    
		    webmediaColl.id = i;//albumNodes[i].getAttribute("description_id");

		    webmediaColl.title = albumNodes[i].getAttribute("name");
		    
		    if (typeof(descriptionItem) !== "undefined" && descriptionItem != null)
		      webmediaColl.description = descriptionItem.getText();
		    
		    webmediaColl.author = user;
		//    webmediaColl.published = albumNodes[i].getAttribute("uploaddate");
		//    webmediaColl.link = albumNodes[i].getElements("url")[0].getText();
		//    webmediaColl.thumbnailLink = albumNodes[i].getElements("thumb")[0].getText();
		    setData(webmediaColl.id, webmediaColl.toArray());
	      //       getPhotoInfo2(webmedia);
		  }
		  catch(e)
		  {
		    print("A problem occured while parsing: " + e.message);
		  }
		}
	      }
	      catch(e)
	      {
		print("A problem occured while parsing: " + e.message);
	      }
	    }
  );
}

Photobucket.prototype.toString = function()
{
  return "Photobucket";
}

Photobucket.prototype.buildUrl = function(url, format) {
  try 
  {
    if( url == undefined ) 
    {
      url = "http://api.photobucket.com/ping";
    }

    if( format == undefined ) 
    {
      format = "format=xml";
    }

    var timestamp = getTimestamp();
    auth_nonce="nonce" + timestamp;

    auth_method = "HMAC-SHA1";
    auth_timestamp = "" + timestamp;

    auth_version="1.0";

    auth_consumer = "&oauth_consumer_key="+ encodeURIComponent( this.consumerKey );
    nonce = "&oauth_nonce="+ encodeURIComponent( auth_nonce );
    auth_sig_method = "&oauth_signature_method="+ encodeURIComponent( auth_method );
    auth_timestamp = "&oauth_timestamp="+ encodeURIComponent( auth_timestamp );
    version = "&oauth_version=" + encodeURIComponent( auth_version );

    paramstring = format + auth_consumer + nonce + auth_sig_method + auth_timestamp + version;

    method = "GET";

    base = encodeURIComponent( method ) + "&" +
    encodeURIComponent( url ) + "&" +
    encodeURIComponent( paramstring );
    print(base);
    sig_hash = getSignature( this.consumerSecret+"&", base );
    auth_sign = "oauth_signature=" + sig_hash;

    auth_url = url + "?" + paramstring + "&" + auth_sign;
    print( ""+ auth_url+"");
    return auth_url;
  }
  catch (err) 
  {
      print( "A problem occured while building the url: " + err.message);
  }
}

Photobucket.prototype.isErrorMessage = function(objDomTree)
{
  if (objDomTree.getElements("status")[0].getText() == "Exception")
  {
    var msg = objDomTree.getElements("message")[0].getText();
    setData(this.toString(), "Error", msg);
    return true;
  }
  return false;
}


function getSignature(key, baseString) {
  b64pad = '=';
  var signature = b64_hmac_sha1(key, baseString);
  return signature;
}

function xmlError(e)
{
  print(e);
}

/*function getPhotoInfo2(webmedia)
{
  var url = this.buildUrl(photobucketObject.baseUrl + "/media/" + encodeURIComponent(webmedia.link) + "/meta", "format=xml");
  print(url);
  var io = photobucketObject.mediaengine.getUrl(url);
  var result = "";
  io.data.connect(function(job, data)
  {
    result += data.valueOf();
  });

  io.finished.connect(function(job)
  {
    var objDom = new XMLDoc(result, xmlError);
    var objDomTree = objDom.docNode;

    webmedia.height = photoNodes = objDomTree.getElements("response")[0].getElements("content")[0].getElements("height")[0].getText();
    webmedia.width = photoNodes = objDomTree.getElements("response")[0].getElements("content")[0].getElements("width")[0].getText();
     setData(webmedia.id, webmedia.toArray());
  });
}*/

function getTimestamp() {
/*  var timestamp = "";
  var io = engine.getUrl("http://api.photobucket.com/time");
  io.data.connect(function(job, data){ timestamp += data.valueOf(); });
  io.finished.connect(function(job){ print(timestamp); return; });
  doRequest(engine, "http://api.photobucket.com/time", function(job, data){ timestamp += data.valueOf(); } , function(job){ print(timestamp); });*/
  
  var timestamp = new Date().valueOf();
  timestamp = timestamp / 1000;
  timestamp = Math.ceil( timestamp );
  return timestamp;
}



registerAddon(Photobucket)


