/**
 * AppController
 * @namespace telepati.controllers
 */
(function () {
    'use strict';
    angular.module('telepati').controller('AppController', AppController);
    AppController.$inject = ['$location', '$scope', '$interval', 'AppService'];
    /**
     * @namespace AppController
     */
    function AppController($location, $scope, $interval, AppService) {
      var vm = this;
      vm.init = init;
      vm.users = Array();
      vm.profile = String();
      vm.showChatWindow = false;
      vm.loadUserChat = loadUserChat;
      vm.sendChatMessage = sendChatMessage;
      vm.setProfile = setProfile;
      vm.profilePing = profilePing;

      /**
      * @name getProfile
      * @desc get profile
      * @memberOf telepati.controllers.AppController
      */
      function getProfile(){
        AppService.getProfile()
          .success(function(data){
            if(data.response){
              vm.profile = data.data.message;
            }
          });
      }

      /**
      * @name getUserProfiles
      * @desc get user list
      * @memberOf telepati.controllers.AppController
      */
      function getUserProfiles(){
        AppService.getUsers()
          .success(function(data){
            vm.users = data.usersList;
          });
      }

      /**
      * @name getUserChats
      * @desc get current user chats
      * @memberOf telepati.controllers.AppController
      */
      function getUserChats(){
        if((vm.user==undefined) || (vm.user.name.length < 1))
          return;

        AppService.getChatMessages(vm.user)
          .success(function(data){
            vm.chats = data.messagesList;
            console.log(vm.chats);
          });
      }

      /**
      * @name init
      * @desc startup script
      * @memberOf telepati.controllers.AppController
      */
      function init() {
        getProfile();
        getUserProfiles();
      }

      /**
      * @name loadUserChat
      * @desc get chat conversation between user
      * @memberOf telepati.controllers.AppController
      */
      function loadUserChat(index) {
        vm.showChatWindow = true;
        vm.user = vm.users[index];
        getUserChats();
      }

      /**
      * @name sendChatMessage
      * @desc send message to chat
      * @memberOf telepati.controllers.AppController
      */
      function sendChatMessage() {
        if((vm.user==undefined) || (vm.user.name.length < 1))
          return;
        if((vm.newMessage==undefined) || (vm.newMessage.length < 1))
          return;

        var msg = {
          name: vm.profile,
          public_key: "public_key",
          message: vm.newMessage,
          recipient: vm.user.name,
        }
        AppService.getChatMessages(msg)
          .success(function(data){
            vm.chats = data.messagesList;
            console.log(vm.chats);
          });
        
        vm.newMessage = "";
      }

      /**
      * @name setProfile
      * @desc set profile name
      * @memberOf telepati.controllers.AppController
      */
      function setProfile() {
        if((vm.profile==undefined) || (vm.profile.length < 1))
          return;

        var msg = {
          name: vm.profile,
        }
        AppService.setProfileName(msg)
          .success(function(data){
            if(data.response){
              getProfile();
              $('#myModal').modal('toggle');
            } 
          });
      }

      /**
      * @name profilePing
      * @desc send profile info to air
      * @memberOf telepati.controllers.AppController
      */
      function profilePing() {
        if((vm.profile==undefined) || (vm.profile.length < 1))
          return;
        
        var msg = {
          name: vm.profile,
          public_key: "public_key",
        }
        AppService.profilePing(msg)
          .success(function(data){
            if(data.response){
              console.log("Ping Successful ...");
            } 
          });
      }

      // $interval(getUserProfiles, 2000);
      // $interval(getUserChats, 2000);
      // $interval(profilePing, 2000);

    }
})();