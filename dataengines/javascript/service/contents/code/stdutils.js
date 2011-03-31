// INET HELPER FUNCTIONS

/*
 * Makes a http request
 * @param webengine: the engine that makes the request
 * @param url: the url to make a request to
 * @param onDataCallBack: function pointer, when data arrives
 * @param onFinishedCallBack: function pointer, when the request finished
 */
function doRequest(webengine, url, onDataCallBack, onFinishedCallBack)
{
  var io = webengine.getUrl(url);
  io.data.connect(onDataCallBack);
  io.finished.connect(onFinishedCallBack);
}

/*
 * Builds an url with query
 * @param baseUrl: the base url to build the url on
 * @param queryItems: array of key value pairs, which represent the query
 * @return a complete url, e.g. http://www.example.com?test=true
 * returns the base url when queryItems is empty
 */
function buildUrl(baseUrl, queryItems)
{
  var newUrl = "";
  if (!queryItems)
    return baseUrl;
  
  if (baseUrl.charAt(baseUrl.length - 1) != '/')
    baseUrl.concat("/");
  newUrl = baseUrl + "?";
  for (var key in queryItems)
  {
    newUrl += key + "=" + queryItems[key] + "&";
  }
  if (newUrl.charAt(newUrl.length - 1) == '&')
    newUrl = newUrl.slice(0, newUrl.length - 1);
  return newUrl;
}

// from http://stackoverflow.com/questions/1657733/compare-two-dates-in-javascript
//function to convert W3C Dateformat in javascript date format
Date.prototype.setISO8601 = function(string) {
    var regexp = "([0-9]{4})(-([0-9]{2})(-([0-9]{2})" +
        "(T([0-9]{2}):([0-9]{2})(:([0-9]{2})(\.([0-9]+))?)?" +
        "(Z|(([-+])([0-9]{2}):([0-9]{2})))?)?)?)?";
    var d = string.match(new RegExp(regexp));

    var offset = 0;
    var date = new Date(d[1], 0, 1);

    if (d[3]) { date.setMonth(d[3] - 1); }
    if (d[5]) { date.setDate(d[5]); }
    if (d[7]) { date.setHours(d[7]); }
    if (d[8]) { date.setMinutes(d[8]); }
    if (d[10]) { date.setSeconds(d[10]); }
    if (d[12]) { date.setMilliseconds(Number("0." + d[12]) * 1000); }
    if (d[14]) {
        offset = (Number(d[16]) * 60) + Number(d[17]);
        offset *= ((d[15] == '-') ? 1 : -1);
    }

    offset -= date.getTimezoneOffset();
    time = (Number(date) + (offset * 60 * 1000));
    this.setTime(Number(time));
}

function UnixToDate(unixTimeStamp)
{
  return new Date(unixTimeStamp * 1000);
}

// OUTPUT FUNCTIONS


var errorID = 0;

function outputErrorMessage(provider, providerSpecificCode, identifier, message)
{
  print(identifier + " caused a problem in " + provider + ": " + message);
  setData("Error:" + errorID, 
	  {
	    "provider" : provider,
	    "provider specific code" : providerSpecificCode,
	    "identifier" : identifier,
	    "message" : message
	  }
  );
  errorID++;
}

var warningID = 0;

function outputWarningMessage(provider, identifier, message)
{
  setData("Warning:" + warningID,
	  {
	    "provider" : provider,
	    "identifier" : identifier,
	    "message" : message
	  }
  );
  warningID++;
}


