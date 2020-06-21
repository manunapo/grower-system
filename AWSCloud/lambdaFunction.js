'use strict';
console.log('Loading hello world function');
var AWS = require('aws-sdk');
var dynamodb = new AWS.DynamoDB({apiVersion: '2012-08-10'});


// Initialising the DynamoDB SDK
const documentClient = new AWS.DynamoDB.DocumentClient();
 
exports.handler = async (event) => {
    
    var messageTmp = "Error: Not a POST";
    var inputTmp = event.body;
    var responseCode = 200;
    
    
    if(event.httpMethod == "POST"){
        if (event.body) {
			var size = 0;
			var body = JSON.parse(event.body);
            if (body.size){
                size = body.size;
			}
			console.log(event.requestContext.requestTime);
			var params = {
				TableName: "Measurements", 
				Item: {
					ID: "data",
					TimeEpoch: event.requestContext.requestTimeEpoch
				}
			};
			for( let i = 1; i <= size; i++){
				let variableName = "data" + i;
				let type = body[variableName].type;
				let value = body[variableName].value;
				params['Item'][type] = value;
			}
			await documentClient.put(params).promise();
			messageTmp = "Value Updated";
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