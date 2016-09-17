(function () {
  'use strict';

  angular
    .module('telepati')
    .factory('AppService', AppService);

  AppService.$inject = ['$http'];

  /**
  * @namespace AppService
  * @returns {Factory}
  */
  function AppService($http) {
    /**
    * @name AppService
    * @desc The Factory to be returned
    */
    var AppServiceFactory = {
      getUsers: getUsers,
      getProfile: getProfile,
      getChatMessages: getChatMessages,
      setProfileName: setProfileName,
      profilePing: profilePing,
    };


    function getUsers(){
      return $http.get('/api/users'); 
    }

    function getProfile(){
      return $http.get('/api/profile'); 
    }

    function getChatMessages(req_param){
      return $http.post('/api/messages', req_param,
                {headers: {'Content-Type': 'application/json'}});
    }

    function sendMessage(req_param){
      return $http.post('/api/messages', req_param,
                {headers: {'Content-Type': 'application/json'}});
    }

    function setProfileName(req_param){
      return $http.post('/api/profile', req_param,
                {headers: {'Content-Type': 'application/json'}});
    }

    function profilePing(req_param){
      return $http.post('/api/ping', req_param,
                {headers: {'Content-Type': 'application/json'}});
    }


    return AppServiceFactory;
  }


})();