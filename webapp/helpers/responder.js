

var createSuccessResponse = function(msg, data){
  var res = Object();
  res.response = true;
  res.data = {
  	message: msg
  }
  if(data!=undefined)
  	res.data.data = data;
  return res;
}

var createErrorResponse = function(msg,params){
  var res = Object();
  res.response = false;

  res.data = {
  	message: msg
  }
  if(params!=undefined)
  	res.data.params = params;

  return res;
}

var responder = {
  successResponse: createSuccessResponse,
  errorResponse: createErrorResponse
}

module.exports = responder