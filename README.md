
<!--
$$\   $$\  $$$$$$\            $$\       
$$ |  $$ |$$  __$$\           $$ |      
$$ |  $$ |\__/  $$ | $$$$$$$\ $$$$$$$\  
$$$$$$$$ | $$$$$$  |$$  _____|$$  __$$\ 
\_____$$ |$$  ____/ \$$$$$$\  $$ |  $$ |
      $$ |$$ |       \____$$\ $$ |  $$ |
      $$ |$$$$$$$$\ $$$$$$$  |$$ |  $$ |
      \__|\________|\_______/ \__|  \__|
-->

42sh Project
=============

<table><tr>
<td>
“Forty-two! Is that all you’ve got to show for seven and a half million years work?”
yelled Loonquawl. “I checked it very thoroughly,” 
said the computer, “and that quite definitely is the answer. 
I think the problem, to be quite honest with you, 
is that you’ve never actually known what the question is.”
--Douglas Adams
</td>
</tr>
</table>

AUTHORS
--------------
* Lea CRUCIANI
* Alexandre GAUTIER
* Matheo BARTHELEMY
* Mathis GUILBAUD

# Building

```sh
meson setup builddir
meson compile -C builddir  # or ninja -C builddir
builddir/42sh
```

# Debugging

```
gdb -arg builddir/42sh -c 'echo test'
```

# Running tests

```sh
tests/run_tests builddir/42sh
```

# Building documentation

```sh
# enable documentation support
meson setup -Ddoc=true builddir  # --reconfigure might be needed
# build the documentation
meson compile -C builddir doxygen_doc
# open the documentation in the browser
xdg-open builddir/doxygen_doc/index.html
```

