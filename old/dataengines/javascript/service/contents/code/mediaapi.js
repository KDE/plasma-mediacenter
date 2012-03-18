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

// MEDIA TYPES

var MediaTypes = new Array();
MediaTypes[PHOTO 		= 0] = "photo";
MediaTypes[VIDEO 		= 1] = "video";


// ERROR TYPES

var ErrorTypes = new Array();
ErrorTypes[NOUSERDEFINED	= 0] = "No user defined.";
ErrorTypes[NOTEXTDEFINED	= 1] = "No text defined.";
ErrorTypes[INVALIDNUMBER	= 2] = "Invalud number.";
ErrorTypes[INVALIDFORMAT	= 3] = "Invalid format.";
ErrorTypes[FUNCTIONNOTPROVIDED	= 4] = "Function not provided.";
ErrorTypes[NORESULTS		= 5] = "No results found.";
ErrorTypes[NOUSERFOUND		= 6] = "No user found.";
ErrorTypes[PARSINGERROR		= 7] = "Error while parsing results";
ErrorTypes[INTERNALERROR	= 8] = "Internal error occured";
// can be continued


// WARNING TYPES

var WarningTypes = new Array();
WarningTypes[PARAMIGNORED	= 0] = "Parameter ignored.";




var nl = '\n';

// THE MEDIA CLASS HOLDS INFORMATION OF A SINGLE MEDIUM

WebMedia = function()
{
  this.type = "";
  this.id = "";
  this.providerSpecificID = "";
  this.provider = "";
  this.description = "";
  this.keywords = new Array();
  this.user = "";
  this.authorNick = "";
  this.updated = "";
  this.published = "";
  this.link = "";
  this.width = "";
  this.height = "";
  // in bytes ?
  this.size = "";
  this.thumbnails = new Array();
  
  //this.rating = 0.0;
  //this.comments = "";
  
  // video specific attributes
  this.videoDuration = 0;
  this.videoFormat = "";
  this.videoEmbedCode = "";
  
}

WebMedia.prototype.toArray = function()
{
  var array = new Array();
  array['type'] = this.type;
  array['provider'] = String(this.provider);
  array['id'] = this.id;
  array['title'] = String(this.title);
  array['user'] = String(this.user);
  array['description'] = String(this.description);
  array['keywords'] = String(this.keywords);
  array['updated'] = this.updated;
  array['published'] = this.published;
  array['link'] = String(this.link);
  array['width'] = parseInt(this.width);
  array['height'] = parseInt(this.height);
  array['size'] = parseInt(this.size);
  for (var i = 0; i < this.thumbnails.length; i++)
    array['thumbnail' + i] = String(this.thumbnails[i].url);
  return array;
}

WebMedia.prototype.toArrayString = function()
{
  var arr = this.toArray();
  for (var key in arr)
  {
    print(key + " : " + arr[key]);
  }
}

WebMedia.prototype.addThumbnail = function(url, height, width)
{
  var thumb = new Thumbnail();
  thumb.url = url;
  thumb.height = height;
  thumb.width = width;
  this.thumbnails.push(thumb);
}


WebMedia.prototype.toString = function()
{
  var res = "Type: " + this.type + nl +
	    "ID: " + this.id + nl + 
	    "Provider specific ID: " + this.providerSpecificID + nl +
	    "Description: " + this.description + nl + 
	    "Keywords: " + nl;
	    for (var i in this.keywords)
	      res += this.keywords[i] + nl;
	    res += "User: " + this.user + nl +
	    "User nickname: " + this.authorNick + nl +
	    "Updated: " + this.updated + nl +
	    "Published: " + this.published + nl +
	    "Link: " + this.link + nl + 
	    "Width: " + this.width + nl +
	    "Height: " + this.height + nl + 
	    "Size: " + this.size + nl + 
	    "Thumbnails: " + nl;
	    for (var i in this.thumbnails)
	      res += "url: " + this.thumbnails[i].url + 
	      ", height: " + this.thumbnails[i].height + 
	      ", width: " + this.thumbnails[i].width + nl;
	    
  return res;
}

Thumbnail = function()
{
  this.url = null;
  this.height = null;
  this.width = null;
}

// CLASS THAT REPRESENTS ALBUMS AND PLAYLISTS

WebMediaCollection = function()
{
  this.id = null;
  this.providerSpecificID = null;
  this.provider = null;
  this.title = null;
  this.user = null;
  this.authorNick = null;
  this.description = null;
  this.updated = null;
  this.published = null;
  this.link = null;
  this.thumbnail = null;
  //this.webmedia = new Array();
}

WebMediaCollection.prototype.toArray = function()
{
  var arr = new Array();
  arr['id'] = this.id;
  arr['provider'] = this.provider;
  arr['title'] = this.title;
  arr['author'] = this.author;
  arr['description'] = this.description;
  arr['published'] = this.published;
  arr['updated'] = this.updated;
  arr['link'] = this.link;
  return arr;
}