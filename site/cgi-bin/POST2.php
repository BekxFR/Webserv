<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $file = $_FILES['file'];
    
    // Check for errors
    if ($file['error'] !== UPLOAD_ERR_OK) {
        // Handle the error
        echo "Error uploading file.";
        exit();
    }
    
    // Get the file data
    $filename = $file['name'];
    $temp_file = $file['tmp_name'];
    $file_size = $file['size'];
    
    // Move the file to a permanent location
    $target_dir = "uploads/";
    $target_file = $target_dir . $filename;
    move_uploaded_file($temp_file, $target_file);
    
    echo "File uploaded successfully.";
}
?>
<form method="post" enctype="multipart/form-data">
    <input type="file" name="file">
    <input type="submit" value="Upload">
</form>
