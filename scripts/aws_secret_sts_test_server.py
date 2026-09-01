#!/usr/bin/env python3
import os
from http.server import BaseHTTPRequestHandler, HTTPServer

RESPONSE = """<?xml version="1.0" encoding="UTF-8"?>
<AssumeRoleResponse xmlns="https://sts.amazonaws.com/doc/2011-06-15/">
  <AssumeRoleResult>
    <Credentials>
      <AccessKeyId>{access_key}</AccessKeyId>
      <SecretAccessKey>ASSUMED_SECRET</SecretAccessKey>
      <SessionToken>ASSUMED_TOKEN</SessionToken>
      <Expiration>2099-01-01T00:00:00Z</Expiration>
    </Credentials>
  </AssumeRoleResult>
  <ResponseMetadata><RequestId>aws-secret-local-repro</RequestId></ResponseMetadata>
</AssumeRoleResponse>"""


class Handler(BaseHTTPRequestHandler):
    def do_POST(self):
        authorization = self.headers.get("Authorization", "")
        if "Credential=SOURCE_KEY/" in authorization:
            access_key = "SOURCE_ASSUMED"
        elif "Credential=WRONG_KEY/" in authorization:
            access_key = "WRONG_ASSUMED"
        else:
            access_key = "UNKNOWN_ASSUMED"

        body = RESPONSE.format(access_key=access_key).encode()
        self.send_response(200)
        self.send_header("Content-Type", "text/xml")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        pass


if __name__ == "__main__":
    port = int(os.environ.get("AWS_SECRET_STS_MOCK_PORT", "8898"))
    HTTPServer(("127.0.0.1", port), Handler).serve_forever()
