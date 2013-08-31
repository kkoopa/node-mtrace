{
  "targets": [
    {
      "target_name": "mtrace",
      "sources": [ "mtrace.cc" ],
      "include_dirs" : ["<!(node -p -e \"require('path').dirname(require.resolve('nan'))\")"]
    }
  ]
}
