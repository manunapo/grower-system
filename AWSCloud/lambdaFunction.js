"body": {
			"size" : 4,
			"data1": {
					"type": "moisure1",
					"value": 1234
			},
			"data2": {
					"type": "level1",
					"value": 1234
			},
			"data3": {
					"type": "bomb1",
					"value": "on"
			},
			"data4": {
					"type": "light1",
					"value": "off"
			}
}

'use strict';
console.log('Loading hello world function');
var AWS = require('aws-sdk');
var dynamodb = new AWS.DynamoDB({apiVersion: '2012-08-10'});
const crypto = require("crypto");
 
// Generate unique id with no external dependencies
const generateUUID = () => crypto.randomBytes(16).toString("hex");

// Initialising the DynamoDB SDK
const documentClient = new AWS.DynamoDB.DocumentClient();
 
exports.handler = async (event) => {
    
    var messageTmp = "Error: Not a POST";
    var inputTmp = event.body;
    var responseCode = 200;
    
    
    if(event.httpMethod == "POST"){
        if (event.body) {
			var size = 0;
			var body = JSON.parse(event.body)
            if (body.size){
                size = body.size;
			}
			for( let i = 1; i <= size; i++){
				let variableName = "data" + i
				let type = body[variableName].type;
				let value = body[variableName].value;
				let time = event.requestContext.requestTime;
				const params = {
					TableName: "Measurements", 
					Item: { 
						ID: generateUUID(),
						Type: type,
						Value: value,
						Time: time
					}
				};
				await documentClient.put(params).promise();
				messageTmp = "Value Updated";
			}
		}
    }
    
    let responseBody = {
        message: messageTmp,
        input: inputTmp
    };
    
    let response = {
        statusCode: responseCode,
        headers: {
            "x-custom-header" : "my custom header value"
        },
        body: JSON.stringify(responseBody)
    };
    console.log("response: " + JSON.stringify(response));
    return response;
   
};