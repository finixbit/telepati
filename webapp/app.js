var express = require('express')
  , app = express()
  , bodyParser = require('body-parser')
  , morgan = require('morgan')
  , expressValidator = require('express-validator')
  , cookieParser = require('cookie-parser')
  , auth = require('./middlewares')
  , port = process.env.PORT || 9002

app.set('views', __dirname + '/views')
app.set('view engine', 'ejs')
app.use('/static', express.static(__dirname + '/public'));
app.use(morgan("dev"));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));
app.use(expressValidator());
app.use(cookieParser());
app.use(require('./controllers'))

app.listen(port, function() {
  console.log('Listening on port ' + port)
})