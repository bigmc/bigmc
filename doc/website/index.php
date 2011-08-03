<?php $title = "Home";
require 'header.php';
?>
<h2>BigMC</h2>
<p>
BigMC (Bigraphical Model Checker) is a model-checker designed to operate on Bigraphical Reactive Systems (BRS). BRS is a formalism developed by Robin Milner and colleagues that emphasises the orthogonal notions of locality and connectivity. Bigraphs have found applications in ubiquitous computing, computational biology, business workflow modelling, and context-aware systems.
</p>
<p>
By model checking, we mean precisely the act of checking whether some specification is true of a particular bigraphical model. This is achieved through a kind of exhaustive search of all the possible states of the system. For arbitrary models, this kind of checking is computationally intractable, as the state space is simply too huge (and indeed infinite in many cases). The challenge of this kind of task is to limit the kinds of models we check to some tractable subset, and to reduce the number of actual states that we need to check directly in order to provide concrete correctness guarantees.
</p>
<h2>Download</h2>
<p>BigMC is released under the terms of the GNU General Public License Version 3</p>
<p>
<ul>
<li><a href="release/bigmc-20110803.tar.gz">BigMC 20110801 (Source tar.gz, 504kb)</a></li>
<li><a href="release/BigMC-20110801.pkg">BigMC 20110801 (MacOS 10.6 Binary Installer Package, 470kb)</a></li>
</ul>
</p>

<h2>Documentation</h2>

<p>
<ul>
<li><a href="manual/">Manual (online, HTML)</a></li>
<li><a href="bigmc.pdf">Manual (offline, PDF, 413kb)</a></li>
</ul>
</p>

<?php require 'footer.php'; ?>
