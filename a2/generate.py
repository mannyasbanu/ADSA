import os


class Node:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.height = 1


def height(node):
    return node.height if node else 0


def update_height(node):
    node.height = 1 + max(height(node.left), height(node.right))


def balance_factor(node):
    return height(node.left) - height(node.right) if node else 0


def rotate_right(root):
    new_root = root.left
    moved_subtree = new_root.right
    new_root.right = root
    root.left = moved_subtree
    update_height(root)
    update_height(new_root)
    return new_root


def rotate_left(root):
    new_root = root.right
    moved_subtree = new_root.left
    new_root.left = root
    root.right = moved_subtree
    update_height(root)
    update_height(new_root)
    return new_root


def rebalance(root):
    update_height(root)
    balance = balance_factor(root)

    if balance > 1:
        if balance_factor(root.left) < 0:
            root.left = rotate_left(root.left)
        return rotate_right(root)

    if balance < -1:
        if balance_factor(root.right) > 0:
            root.right = rotate_right(root.right)
        return rotate_left(root)

    return root


def insert(root, key):
    if root is None:
        return Node(key)

    if key < root.key:
        root.left = insert(root.left, key)
    else:
        root.right = insert(root.right, key)

    return rebalance(root)


def min_node(root):
    cur = root
    while cur.left is not None:
        cur = cur.left
    return cur


def delete(root, key):
    if root is None:
        return None

    if key < root.key:
        root.left = delete(root.left, key)
    elif key > root.key:
        root.right = delete(root.right, key)
    else:
        if root.left is None:
            return root.right
        if root.right is None:
            return root.left

        successor = min_node(root.right)
        root.key = successor.key
        root.right = delete(root.right, successor.key)

    return rebalance(root)


def preorder(root):
    if root is None:
        return []
    return [root.key] + preorder(root.left) + preorder(root.right)


def inorder(root):
    if root is None:
        return []
    return inorder(root.left) + [root.key] + inorder(root.right)


def postorder(root):
    if root is None:
        return []
    return postorder(root.left) + postorder(root.right) + [root.key]


def format_traversal(values):
    return "EMPTY" if not values else " ".join(str(v) for v in values)


def run_case(contents):
    root = None
    outputs = []

    for token in contents.split():
        if token.startswith("A"):
            root = insert(root, int(token[1:]))
        elif token.startswith("D"):
            root = delete(root, int(token[1:]))
        elif token == "PRE":
            outputs.append(format_traversal(preorder(root)))
        elif token == "IN":
            outputs.append(format_traversal(inorder(root)))
        elif token == "POST":
            outputs.append(format_traversal(postorder(root)))
        else:
            raise ValueError(f"Unsupported token: {token}")

    return "\n".join(outputs)


os.makedirs("expected", exist_ok=True)

for file_name in sorted(os.listdir("tests")):
    test_path = os.path.join("tests", file_name)
    with open(test_path, "r", encoding="utf-8") as test_file:
      contents = test_file.read().strip()

    expected = run_case(contents)

    output_path = os.path.join("expected", file_name)
    with open(output_path, "w", encoding="utf-8") as output_file:
        output_file.write(expected)
        if expected:
            output_file.write("\n")
