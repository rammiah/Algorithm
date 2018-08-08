public class TreeToList {
    // 将二叉搜索树转换为有序链表
    static class Node {
        int val;
        Node left, right;

        Node(int val) {
            this.val = val;
            left = right = null;
        }

        public Node(int val, Node left, Node right) {
            this.val = val;
            this.left = left;
            this.right = right;
        }
    }

    Node treeToList(Node root) {
        if (root == null) {
            return null;
        }
        Node left = treeToList(root.left);
        Node right = treeToList(root.right);
        Node head;
        // 4种情况
        // 1. 左空右空
        if (left == null && right == null) {
            head = root;
        } else if (left != null && right == null) {
            //2. 右空
            head = left;
            Node n = left;
            while (n.right != null) {
                n = n.right;
            }
            n.right = root;
            root.left = n;
        } else if (left == null) {
            // 左空
            head = root;
            root.right = right;
            right.left = root;
        } else {
            // 都不空
            head = left;
            Node n = left;
            while (n.right != null) {
                n = n.right;
            }
            n.right = root;
            root.left = n;
            root.right = right;
            right.left = root;
        }
        return head;
    }


    public static void main(String[] args) {
        TreeToList tt =  new TreeToList();
        /*
        *         10
        *       /   \
        *      6    14
        *     / \  /  \
        *    4  8 12  16
        * 
        * 4-6-8-10-12-14-16
        * */
        Node n4 = new Node(4);
        Node n8 = new Node(8);
        Node n12 = new Node(12);
        Node n16 = new Node(16);
        Node n6 = new Node(6, n4, n8);
        Node n14 = new Node(14, n12, n16);
        Node n10 = new Node(10, n6, n14);
        Node result = tt.treeToList(n10);
        Node n = result;
        while (n != null) {
            System.out.println(n.val);
            n = n.right;
        }
    }
}
