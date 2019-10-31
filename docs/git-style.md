# git-style

## Feature development

Every new **feature**/**documentation** patch should first be proposed as an issue for latter development on a **new branch**.

## Branch naming convention

To name a new branch, follow the pattern:

```
type/issue#/objective
```

where type may be:
1. fature
2. bug
3. hotfix
4. test
5. build
6. docs

and issue# is the number given by github to the associated issue.

lastly, objective should be a short, descriptive name related to the issue's goal.

an exaple of a possible bug-related branch could be:

```
bug/15/product_reactant_factoring
```

## Code merging convention

After a branch's purpose has been fulfilled, the next step in integrating the changes is creating a pull request and assigning another developer to review it.