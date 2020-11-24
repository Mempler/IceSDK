import sys


def main():
    target = sys.argv[1]

    with open(target + ".html", 'w') as f:
        f.write("""
<html>

<body>
    <canvas id="canvas"></canvas>

    <script>
        var Module = {}
    </script>

    <script src="%s.fs.js"></script>
    <script src="%s.js"></script>
</body>

</html>
""" % (target, target))


if __name__ == "__main__":
    main()
