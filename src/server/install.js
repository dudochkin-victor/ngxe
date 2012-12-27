var util = require('util');
var npm = require('npm');
util.log('Installing required modules');
npm.load({}, function (er) {
  if (er) return handlError(er);
  npm.commands.install(['jsonrpc', 'jqtpl', 'mysql-libmysqlclient@1.4.0', 'libxmljs@0.4.3'], function (er, data) {
    if (er) util.log(er);
  });
});