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

engine.include('xmldom.js');
engine.include('xmlsax.js');
engine.include('xmlw3cdom.js');
engine.include('sha1.js');
engine.include('stdutils.js');
engine.include('mediaapi.js');

var addons = new Array();
var selectedAddon = null;
var webmediaID = 0;
print("Hello");
function getID()
{
  return webmediaID++;
}

// callback function when addon was created
function addonCreated(addon)
{
  print("new addon: " + addon)
  addons.push(addon);
  setData(addonsObject[i].name, 'text', '');
  print(addons.length);
}

engine.addEventListener("addonCreated", addonCreated);

var addonsObject = engine.listAddons("org.kde.plasma.dataengine.webmedia");

for (i in addonsObject) 
{
  print("Addon: " + addonsObject[i].name);
  engine.loadAddon("org.kde.plasma.dataengine.webmedia", addonsObject[i].id);
}

function GetAddon(name)
{
  print("Searching for " + name);
  for (var i = 0; i < addons.length; i++)
  {
    if (addons[i] == name)
    {
      print("I found " + name);
      return addons[i];
    }
  }
  return null;
}

// PARAMETER CHECK FUNCTIONS FOR EACH FUNCTION IN THE INTERFACE
// ALL OF THE RETURN A NEW ARRAY OF PARAMETERS
// OPTIONAL PARAMETERS GET DEFAULT VALUES

function checkSearchMediaParams(provider, params)
{
  // new query parameters array that holds all parameters with (default) values
  var newParams = new Array();
 
  var text = params['text'];
  var maxResults = params['max-results'];
  var page = params['page'];
  var media = params['media'];
  
  if (text == "" || !text)
  {
    outputErrorMessage(provider, "", "text", ErrorTypes[NOTEXTDEFINED]);
    return null;
  }
  
  if (maxResults == "" || !maxResults)
    // TODO set maxResults with config value
    maxResults = 25;
  else
  {
    if (isNaN(maxResults))
    {
      outputErrorMessage(provider, "", maxResults, ErrorTypes[INVALIDNUMBER]);
      return null;
    }
    maxResults = parseInt(maxResults);
  }
   
  if (page == "" || !page)
    page = 1;
  else{
    if (isNaN(page))
    {
      outputErrorMessage(provider, "", page, ErrorTypes[INVALIDNUMBER]);
      return null;
    }
    // TODO set page with config value
    page = parseInt(page);
  }
  
  if (media != "" || media)
  {
    // Workaround check for valid format
    var index = 0;
    media = media.toLowerCase();
    for(var key in MediaTypes)
    {
      if (media == MediaTypes[key])
	break;
      index++;
    }
    // all keys in MediaTypes are traversed
    if (index == MediaTypes.length - 1)
      outputErrorMessage(provider, "", media, ErrorTypes[INVALIDFORMAT]);
  }
  
  newParams['text'] = text;
  newParams['max-results'] = maxResults;
  newParams['page'] = page;
  newParams['media'] = media;
  return newParams;
}

function checkSearchCollectionParams(provider, params)
{
  var newParams = new Array();
  var text = params['text'];
  
  if (text == "" || !text)
  {
    outputErrorMessage(provider, "", ErrorTypes[NOTEXTDEFINED]);
    return null;
  }
  newParams['text'] = text;
  return queryParams;
}

function checkUploadedUserMediaParams(provider, params)
{
  var newParams = new Array();
  
  var user = params['user'];
  var maxResults = params['max-results'];
  var page = params['page'];
  var collectionID = params['providerCollectionID'];
  
  if (user == "" || !user)
  {
    outputErrorMessage(provider, "", user, ErrorTypes[NOUSERDEFINED]);
    return null;
  }
  if (maxResults == "" || !maxResults)
    maxResults = 25;
  else{
    if (isNaN(maxResults))
    {
      outputErrorMessage(provider, "", maxResults, ErrorTypes[INVALIDNUMBER]);
      return null;
    }
    maxResults = parseInt(maxResults);
  }
  if (page == "" || !page)
    page = 1;
  else
  {
    if (isNaN(page))
    {
      outputErrorMessage(provider, "", page, ErrorTypes[INVALIDNUMBER]);
      return null;
    }
    page = parseInt(page);
  }
  
  newParams['user'] = user;
  newParams['max-results'] = maxResults;
  newParams['page'] = page;
  newParams['providerCollectionID'] = collectionID;
  return newParams;
}

function checkUploadedUserCollectionParams(provider, params)
{
  var newParams = new Array();
  
  var user = params['user'];
  var maxResults = params['max-results'];
  var page = params['page'];
  
  if (user == "" || !user)
  {
    outputErrorMessage(provider, "", user, ErrorTypes[NOUSERDEFINED]);
    return null;
  }
  
  if (maxResults == "" || !maxResults)
    maxResults = 25;
  else
  {
    if (isNaN(maxResults))
    {
      outputErrorMessage(provider, "", maxResults, ErrorTypes[INVALIDNUMBER]);
      return null;
    }
    maxResults = parseInt(maxResults);
  }
  
  if (page == "" || !page)
    page = 1;
  else
  {
    if (isNaN(page))
    {
      outputErrorMessage(provider, "", page, ErrorTypes[INVALIDNUMBER]);
      return null;
    }
    page = parseInt(page);   
  }
  
  newParams['user'] = user;
  newParams['max-results'] = maxResults;
  newParams['page'] = page;
  return newParams;
}


function setupJob(job)
{
  job.addon = selectedAddon;
  if (job.operationName == 'searchMedia')
  {
    job.start = function()
    {
      print("Service starts a request to " + this.addon + " with " + this.parameters.length + " parameters");
      var params = checkSearchMediaParams(this.addon.name, this.parameters);
      
      if (params){
	print("params not null");

	this.addon.searchMedia(params);
	this.result = true;      
      } else
      {
	this.result = false;
	print("params null");
      }
    };
  }
  if (job.operationName == 'searchCollection')
  {
    job.start = function()
    {
      print("Service starts a request to " + this.addon + " with " + this.parameters.length + " parameters");
      var params = checkSearchCollectionParams(this.addon.name, this.parameters);
      if (params)
      {
	this.addon.searchCollection(queryParams);
	this.result = true;
      }else
	this.result = false;
    };
  }
  if (job.operationName == 'getUploadedUserMedia')
  {
    job.start = function()
    {
      print("Service starts a request to " + this.addon + " with " + this.parameters.length + " parameters");
      var params = checkUploadedUserMediaParams(this.addon.name, this.parameters);
      if (params)
      {
	this.addon.getUploadedUserMedia(params);
	this.result = true;
      }
      else
	this.result = false;
    };
  }
  if (job.operationName == 'getUploadedUserCollection')
  {
    job.start = function()
    {
      print("Service starts a request to " + this.addon + " with " + this.parameters.length + " parameters");
      var params = checkUploadedUserCollectionParams(this.addon.name, this.parameters);
      if (params)
      {
	this.addon.getUploadedUserCollection(params);
	this.result = true;
      }else
	this.result = false;
    };
  }
}

engine.serviceForSource = function(source)
{
  selectedAddon = GetAddon(source);
  if (selectedAddon != null)
  {
      var service = new Service('provider');
      service.setupJob = setupJob;
      return service;
  }
}
