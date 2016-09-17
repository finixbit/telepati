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

      function getProfile(){
        AppService.getProfile()
          .success(function(data){
            if(data.response){
              vm.profile = data.data.message;
            }
          });
      }

      function getUserProfiles(){
        AppService.getUsers()
          .success(function(data){
            vm.users = data.usersList;
          });
      }

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
      * @desc get profile / user list
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
        console.log(msg);
        return;
        AppService.getChatMessages(msg)
          .success(function(data){
            vm.chats = data.messagesList;
            console.log(vm.chats);
          });
        
        vm.newMessage = "";
      }

      // $interval(getUserProfiles, 2000);
      // $interval(getUserChats, 2000);

    }
})();