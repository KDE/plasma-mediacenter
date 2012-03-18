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
  this.baseUrl = "http://api.photobucket.com/";
  this.name = "photobucket";
  photobucketObject = this;
}

Photobucket.prototype.searchMedia = function(queryParams)
{
  var text = queryParams['text'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  var media = queryParams['media'];
  
  if (maxResults != "")
    outputWarningMessage(this.name, maxResults, WarningTypes[PARAMIGNORED]);
  // set default value for media. default value is photo
  if (!media || media == "" || media == MediaTypes[PHOTO])
    media = "image";
  if (media == MediaTypes[VIDEO])
    media = "video";
  
  this.buildPhotobucketUrl(this.baseUrl + "search/" + text + "/" + media, 
      {
	"page" : page
      },
      function(url)
      {
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
		      print("Parsing...");
		      print(result);
		      var objDom = new XMLDoc(result, xmlError);
		      var objDomTree = objDom.docNode;

		      if (photobucketObject.isErrorMessage(objDomTree))
			return;
		      
		      var photoNodes = objDomTree.getElements("content")[0].getElements("result")[0].getElements("primary")[0].getElements("media");
		      if (photoNodes.length == 0)
		      {
			outputErrorMessage(photobucketObject.name, "", text, ErrorTypes[NORESULTS]);
			return;
		      }
		      print(photoNodes.length);

		      for (var i = 0; i < photoNodes.length; i++)
		      {
			var webmedia = new WebMedia();
			
			//potential nullable items
			var descriptionItem = photoNodes[i].getElements("decription")[0];
			
			var typeItem = photoNodes[i].getElements("type")[0];
			if (typeItem && typeItem.getText() == "video")
			  webmedia.type = MediaTypes[VIDEO];
			else
			  webmedia.type = MediaTypes[PHOTO];
			
			//in photobucket the attribute 'description_id' is somehow always an empty string
			webmedia.id = i;
			webmedia.providerSpecificID = photoNodes[i].getAttribute("description_id");

			webmedia.title = photoNodes[i].getElements("title")[0].getText();
			
			if (descriptionItem)
			  webmedia.description = descriptionItem.getText();
			
			webmedia.user = photoNodes[i].getAttribute("username");
			webmedia.published = UnixToDate(photoNodes[i].getAttribute("uploaddate"));
			webmedia.link = photoNodes[i].getElements("url")[0].getText();
			webmedia.addThumbnail(photoNodes[i].getElements("thumb")[0].getText(), "", "");
			setData(webmedia.id, webmedia.toArray());
		      }
		    }
		    catch(e)
		    {
		      outputErrorMessage(photobucketObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
		    }
		  }
	  );  
      }
  );
  
}


Photobucket.prototype.searchCollection = function(queryParams)
{
  var text = queryParams['text'];
  var result = "";
  this.buildPhotobucketUrl(this.baseUrl + "search/" + text + "/group", 
			   null,
			   function(url)
			   {
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
					  var groupNodes = objDomTree.getElements("content")[0].getElements("album");
					  if (groupNodes.length == 0)
					  {
					    outputErrorMessage(photobucketObject.name, "", text, ErrorTypes[NORESULTS]);
					    return;
					  }
					  for (var i = 0; i < groupNodes.length; i++)
					  {
					    var webmediaColl = new WebMediaCollection();
					    
					    //potential nullable items
					    var descriptionItem = groupNodes[i].getElements("decription")[0];
					    
					    webmediaColl.id = getID();//groupNodes[i].getAttribute("description_id");
					    
					    webmediaColl.providerSpecificID = groupNodes[i].getElements("id")[0].getText();
					    webmediaColl.title = groupNodes[i].getAttribute("name");
					    
					    if (typeof(descriptionItem) !== "undefined" && descriptionItem != null)
					      webmediaColl.description = descriptionItem.getText();
					    
					  //  webmediaColl.author = user;
					//    webmediaColl.published = groupNodes[i].getAttribute("uploaddate");
					//    webmediaColl.link = groupNodes[i].getElements("url")[0].getText();
					//    webmediaColl.thumbnailLink = groupNodes[i].getElements("thumb")[0].getText();
					    setData(webmediaColl.id, webmediaColl.toArray());
				      //       getPhotoInfo2(webmedia);
					  }
					}
					catch(e)
					{
					  outputErrorMessage(photobucketObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
					}
				      }
			    );
			   }
       );
}

Photobucket.prototype.getUserUploadedMedia = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  
  this.buildPhotobucketUrl(this.baseUrl + "/user/" + user + "/search",
			   {
			    "perpage" : maxResults
			   },
			   function(url)
			   {
			      var result = "";
	  
			      doRequest(engine, url,
					function(job, data)
					{
					  result += data.valueOf();
					},
					function(job)
					{
					  try{
					    print("Parsing...");
					    print(result);
					    var objDom = new XMLDoc(result, xmlError);
					    var objDomTree = objDom.docNode;

					    if (photobucketObject.isErrorMessage(objDomTree))
					      return;
					    
					    var photoNodes = objDomTree.getElements("content")[0].getElements("result")[0].getElements("primary")[0].getElements("media");
					    if (photoNodes.length == 0)
					    {
					      outputErrorMessage(photobucketObject.name, "", text, ErrorTypes[NORESULTS]);
					      return;
					    }
					    print(photoNodes.length);

					    for (var i = 0; i < photoNodes.length; i++)
					    {
					    
					      var webmedia = new WebMedia();
					      
					      //potential nullable items
					      var descriptionItem = photoNodes[i].getElements("decription")[0];
					      
					      var typeItem = photoNodes[i].getElements("type")[0];
					      if (typeItem && typeItem.getText() == "video")
						webmedia.type = MediaTypes[VIDEO];
					      else
						webmedia.type = MediaTypes[PHOTO];
					      
					      //in photobucket the attribute 'description_id' is somehow always an empty string
					      webmedia.id = i;
					      webmedia.providerSpecificID = photoNodes[i].getAttribute("description_id");

					      webmedia.title = photoNodes[i].getElements("title")[0].getText();
					      
					      if (descriptionItem)
						webmedia.description = descriptionItem.getText();
					      
					      webmedia.user = photoNodes[i].getAttribute("username");
					      webmedia.published = UnixToDate(photoNodes[i].getAttribute("uploaddate"));
					      webmedia.link = photoNodes[i].getElements("url")[0].getText();
					      webmedia.addThumbnail(photoNodes[i].getElements("thumb")[0].getText(), "", "");
					      setData(webmedia.id, webmedia.toArray());
					    }
					  }
					  catch(e)
					  {
					    outputErrorMessage(photobucketObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
					  }
					}
				);  
			    }
       );
}

Photobucket.prototype.getUserUploadedCollection = function(queryParams)
{
  var user = queryParams['user'];
  var maxResults = queryParams['max-results'];
  var page = queryParams['page'];
  
  
  this.buildPhotobucketUrl(this.baseUrl + "/user/" + user + "/group",
			   null,
			   function(url)
			   {
			      var result = "";
	  
			      doRequest(engine, url,
					function(job, data)
					{
					  result += data.valueOf();
					},
					function(job)
					{
					  try{
					    print("Parsing...");
					    print(result);
					    var objDom = new XMLDoc(result, xmlError);
					    var objDomTree = objDom.docNode;

					    if (photobucketObject.isErrorMessage(objDomTree))
					      return;
					    
					    var photoNodes = objDomTree.getElements("content")[0].getElements("result")[0].getElements("primary")[0].getElements("media");
					    if (photoNodes.length == 0)
					    {
					      outputErrorMessage(photobucketObject.name, "", text, ErrorTypes[NORESULTS]);
					      return;
					    }
					    print(photoNodes.length);

					    for (var i = 0; i < photoNodes.length; i++)
					    {
					      var webmedia = new WebMedia();
					      
					      //potential nullable items
					      var descriptionItem = photoNodes[i].getElements("decription")[0];
					      
					      var typeItem = photoNodes[i].getElements("type")[0];
					      if (typeItem && typeItem.getText() == "video")
						webmedia.type = MediaTypes[VIDEO];
					      else
						webmedia.type = MediaTypes[PHOTO];
					      
					      //in photobucket the attribute 'description_id' is somehow always an empty string
					      webmedia.id = i;
					      webmedia.providerSpecificID = photoNodes[i].getAttribute("description_id");

					      webmedia.title = photoNodes[i].getElements("title")[0].getText();
					      
					      if (descriptionItem)
						webmedia.description = descriptionItem.getText();
					      
					      webmedia.user = photoNodes[i].getAttribute("username");
					      webmedia.published = UnixToDate(photoNodes[i].getAttribute("uploaddate"));
					      webmedia.link = photoNodes[i].getElements("url")[0].getText();
					      webmedia.addThumbnail(photoNodes[i].getElements("thumb")[0].getText(), "", "");
					      setData(webmedia.id, webmedia.toArray());
					    }
					  }
					  catch(e)
					  {
					    outputErrorMessage(photobucketObject.name, "", e.message, ErrorTypes[PARSINGERROR]);
					  }
					}
				);  
			    }
       );
}

Photobucket.prototype.toString = function()
{
  return "Photobucket";
}

Photobucket.prototype.buildPhotobucketUrl = function(url, optionalParams, onUrlCreated)
{
  var pbUrl = "";
  this.getTimestamp(
    function(timestamp)
    {
      var params = {
		      "format" : "xml",
		      "oauth_consumer_key" : encodeURIComponent(photobucketObject.consumerKey),
		      "oauth_nonce" : encodeURIComponent("nonce" + timestamp),
		      "oauth_signature_method" : encodeURIComponent("HMAC-SHA1"),
		      "oauth_timestamp" : encodeURIComponent(timestamp),
		      "oauth_version" : encodeURIComponent("1.0")
		    };
      if (optionalParams)
      {
	for (var key in optionalParams)
	  params[key] = encodeURIComponent(optionalParams[key]);
      }
      var query = photobucketObject.makeQuery(params);
      //print(encodeURIComponent(query));
      var base = encodeURIComponent("GET") + "&" + encodeURIComponent(url) + "&" + encodeURIComponent(query);
      print(base);
      var base2 = encodeURIComponent("GET") + "&" + url + "&" + query;
      print(base2);
      params["oauth_signature"] = getSignature(photobucketObject.consumerSecret + "&", base);

    //  print(photobucketObject.makeQuery(params));
      pbUrl = buildUrl(url, params);
      print(pbUrl);
      onUrlCreated(pbUrl);
    }
  );
}

Photobucket.prototype.makeQuery = function(queryItems)
{
  var query = "";
  for (var key in queryItems)
  {
    query += key + "=" + queryItems[key] + "&";
  }
  if (query.charAt(query.length - 1) == '&')
    query = query.slice(0, query.length - 1);
  return query;
}

Photobucket.prototype.isErrorMessage = function(objDomTree)
{
  try
  {
    if (objDomTree.getElements("status")[0].getText() == "Exception")
    {
      var msg = objDomTree.getElements("message")[0].getText();
      outputErrorMessage(photobucketObject.name, "", "Server response", msg);
      return true;
    }
    return false;
  }catch(e)
  {
    outputErrorMessage(this.name, "", e.message, ErrorTypes[PARSINGERROR]);
    return false;
  }
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

Photobucket.prototype.getTimestamp = function(onTimeStampRetrieved) {
/*  var timestamp = "";
  var io = engine.getUrl("http://api.photobucket.com/time");
  io.data.connect(function(job, data){ timestamp += data.valueOf(); });
  io.finished.connect(function(job){ print(timestamp); return; });
  doRequest(engine, "http://api.photobucket.com/time", function(job, data){ timestamp += data.valueOf(); } , function(job){ print(timestamp); });*/
  
  /*var timestamp = new Date().valueOf();
  timestamp = timestamp / 1000;
  timestamp = Math.ceil( timestamp );
  return timestamp;*/
  var timestamp = "";
  var url = buildUrl(this.baseUrl + "time", null);
  print(url);
  
  doRequest(engine, url,
	    function(job, data)
	    {
	      timestamp += data.valueOf();
	    },
	    function(job)
	    {
	      print(timestamp);
	      onTimeStampRetrieved(timestamp);
	    }
  );
}



registerAddon(Photobucket)


